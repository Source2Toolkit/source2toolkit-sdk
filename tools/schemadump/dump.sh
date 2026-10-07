#!/usr/bin/env bash
# Source2Toolkit SDK -- dump the CS2 server schema without the game content.
#
#   tools/schemadump/dump.sh <work dir> <output json>
#
# Sets up a dedicated server from the Linux binaries (depot 2347773) plus the
# few files it needs before Metamod loads its plugins (from depot 2347770):
# the gameinfo files, csgo/cfg/user_keys_default.vcfg, and csgo/gamemodes.txt,
# which only exists inside pak01 -- only the one archive holding it is fetched.
# Metamod (latest 2.0 drop), Source2SchemaDumper (latest release) and the runner
# plugin (runner/, built here) are installed, and the server is started: the
# runner dumps in its Load() and exits the process before the server would
# crash on the missing content. About 1 GB is downloaded.
#
# Needs MMSOURCE_DEV and s2sdk (to build the runner -- an S2SDK / HL2SDKCS2
# variable or the vendor/s2sdk submodule), cmake, a C++20 compiler, curl,
# python3 with the `vpk` module. DEPOTDOWNLOADER may point to a
# DepotDownloader binary; otherwise the latest Linux release is fetched.
set -euo pipefail

[ $# -eq 2 ] || { echo "usage: $0 <work dir> <output json>" >&2; exit 1; }
: "${MMSOURCE_DEV:?MMSOURCE_DEV is not set}"

WORK=$(realpath -m "$1")
OUT=$(realpath -m "$2")
HERE=$(cd "$(dirname "$0")" && pwd)
SERVER="$WORK/server"
GAME="$SERVER/game"

APP=730
DEPOT_BINARIES=2347773
DEPOT_CONTENT=2347770

mkdir -p "$WORK" "$SERVER"

# --- DepotDownloader ---------------------------------------------------------
if [ -z "${DEPOTDOWNLOADER:-}" ]; then
    echo "=== Fetching DepotDownloader"
    url=$(curl -fsSL https://api.github.com/repos/SteamRE/DepotDownloader/releases/latest \
        | python3 -c 'import json,sys; print(next(a["browser_download_url"] for a in json.load(sys.stdin)["assets"] if a["name"] == "DepotDownloader-linux-x64.zip"))')
    curl -fsSL -o "$WORK/dd.zip" "$url"
    python3 -m zipfile -e "$WORK/dd.zip" "$WORK/depotdownloader"
    DEPOTDOWNLOADER="$WORK/depotdownloader/DepotDownloader"
    chmod +x "$DEPOTDOWNLOADER"
fi

# depot_download <depot> <filelist lines...>
depot_download() {
    local depot=$1; shift
    printf '%s\n' "$@" > "$WORK/filelist_$depot.txt"
    "$DEPOTDOWNLOADER" -app "$APP" -depot "$depot" -filelist "$WORK/filelist_$depot.txt" \
        -dir "$SERVER" -max-downloads 8 > "$WORK/depotdownloader_$depot.log" \
        || { tail -20 "$WORK/depotdownloader_$depot.log"; exit 1; }
}

# --- Server files ------------------------------------------------------------
echo "=== Downloading the Linux binaries (depot $DEPOT_BINARIES)"
depot_download "$DEPOT_BINARIES" \
    'regex:^game/bin/linuxsteamrt64/.*' \
    'regex:^game/csgo/bin/linuxsteamrt64/.*' \
    'regex:^game/core/bin/linuxsteamrt64/.*'

echo "=== Downloading the gameinfo/config files (depot $DEPOT_CONTENT)"
depot_download "$DEPOT_CONTENT" \
    'regex:^game/(csgo|core|csgo_core|csgo_imported)/gameinfo(_branchspecific)?\.gi$' \
    'game/csgo/cfg/user_keys_default.vcfg' \
    'game/csgo/steam.inf' \
    'game/csgo/pak01_dir.vpk'

echo "=== Extracting csgo/gamemodes.txt from pak01"
archive=$(python3 - "$GAME/csgo/pak01_dir.vpk" <<'PY'
import sys, vpk
print(vpk.open(sys.argv[1]).get_file_meta("gamemodes.txt")["archive_index"])
PY
)
if [ "$archive" != "32767" ]; then # 0x7fff: stored in the _dir file itself
    depot_download "$DEPOT_CONTENT" "$(printf 'game/csgo/pak01_%03d.vpk' "$archive")"
fi
python3 - "$GAME/csgo/pak01_dir.vpk" "$GAME/csgo/gamemodes.txt" <<'PY'
import sys, vpk
vpk.open(sys.argv[1]).get_file("gamemodes.txt").save(sys.argv[2])
PY
rm -f "$GAME"/csgo/pak01_*.vpk

chmod +x "$GAME/bin/linuxsteamrt64/cs2"

# --- Metamod, dumper, runner -------------------------------------------------
echo "=== Installing Metamod:Source (latest 2.0 drop)"
mms=$(curl -fsSL https://mms.alliedmods.net/mmsdrop/2.0/mmsource-latest-linux)
curl -fsSL -o "$WORK/mmsource.tar.gz" "https://mms.alliedmods.net/mmsdrop/2.0/$mms"
tar -xzf "$WORK/mmsource.tar.gz" -C "$GAME/csgo"
echo "Metamod: $mms"

# Metamod is loaded through a search path in front of csgo.
gameinfo="$GAME/csgo/gameinfo.gi"
if ! grep -q 'csgo/addons/metamod' "$gameinfo"; then
    awk '{ print } /Game_LowViolence/ { print "\t\t\tGame\tcsgo/addons/metamod" }' "$gameinfo" > "$gameinfo.tmp"
    mv "$gameinfo.tmp" "$gameinfo"
fi
grep -q 'csgo/addons/metamod' "$gameinfo" || { echo "Could not patch $gameinfo for Metamod" >&2; exit 1; }

echo "=== Installing Source2SchemaDumper (latest release)"
url=$(curl -fsSL https://api.github.com/repos/GAMMACASE/Source2SchemaDumper/releases/latest \
    | python3 -c 'import json,sys; r=json.load(sys.stdin); print(next(a["browser_download_url"] for a in r["assets"] if "linux" in a["name"] and a["name"].endswith(".zip")))')
curl -fsSL -o "$WORK/schemadump.zip" "$url"
python3 -m zipfile -e "$WORK/schemadump.zip" "$GAME/csgo"
echo "Dumper: $(basename "$url")"

echo "=== Building the runner"
cmake -S "$HERE/runner" -B "$WORK/runner-build" > /dev/null
cmake --build "$WORK/runner-build" > /dev/null
mkdir -p "$GAME/csgo/addons/schemadump_runner/bin/linuxsteamrt64"
cp "$WORK/runner-build/schemadump_runner.so" "$GAME/csgo/addons/schemadump_runner/bin/linuxsteamrt64/"

# metaplugins.ini instead of .vdf files: it keeps the order, and the dumper
# has to be loaded before the runner.
rm -f "$GAME"/csgo/addons/metamod/*.vdf
printf '%s\n' \
    'addons/schemadump/bin/linuxsteamrt64/schemadump' \
    'addons/schemadump_runner/bin/linuxsteamrt64/schemadump_runner' \
    > "$GAME/csgo/addons/metamod/metaplugins.ini"

# --- Dump --------------------------------------------------------------------
echo "=== Dumping"
rm -rf "$GAME/csgo/addons/schemadump/dumps"
(
    cd "$GAME/bin/linuxsteamrt64"
    LD_LIBRARY_PATH="$PWD" timeout 300 ./cs2 -dedicated -console -port 27099 +sv_lan 1 \
        > "$WORK/server.log" 2>&1 || true
)
if ! grep -q 'schemadump_runner: done' "$WORK/server.log"; then
    echo "The runner did not finish, server log:" >&2
    tail -40 "$WORK/server.log" >&2
    exit 1
fi

json=$(find "$GAME/csgo/addons/schemadump/dumps" -name '*.json' | head -1)
python3 - "$json" <<'PY'
import json, sys
d = json.load(open(sys.argv[1]))
server = sum(1 for c in d["defs"] if c.get("scope") in ("libserver.so", "server.dll"))
print(f"defs: {len(d['defs'])}, server classes/enums: {server}, game: {d['game_info'].get('PatchVersion', '?').strip()}")
if server < 100:
    sys.exit("The dump has almost no server types, something went wrong")
PY
mkdir -p "$(dirname "$OUT")"
cp "$json" "$OUT"
echo "Wrote $OUT"
