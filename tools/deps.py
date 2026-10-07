#!/usr/bin/env python3
# vim: set ts=4 sw=4 tw=99 et:
"""
Source2Toolkit SDK -- dependency setup.

The SDK compiles against:

  * s2sdk               -- AlliedModders' s2sdk (cs2 branch, formerly hl2sdk's
                           cs2 branch); vendor/s2sdk, a submodule, unless you
                           point at your own checkout
  * Protobufs (csgo/)   -- SteamTracking's protobuf definitions
  * KHook               -- vendor/khook, a submodule, which has to be the very
                           commit metamod-source was built with or the toolkit
                           core refuses every plugin at load time
  * hl2sdk-manifests    -- vendor/hl2sdk-manifests, a submodule (AMBuild only)

`init` asks, once, where each of the first two comes from: a checkout you
already have (found through an environment variable -- S2SDK_CS2, S2SDKCS2,
S2SDK, HL2SDK_CS2 or HL2SDKCS2, first set wins / CSGO_PROTO -- or a path you
type), or this repo: the vendor/s2sdk submodule, a Protobufs clone under
vendor/. The answer is saved in deps.json next to this SDK, and `update` then
brings everything up to date the same way every time: pulls your checkouts,
puts the submodule on the commit this SDK pins, moves KHook to the commit
metamod-source pins, moves the manifests to their latest.

Without deps.py at all, CMake and AMBuild take s2sdk from the environment when
one of those variables is set and from vendor/s2sdk otherwise.

    python tools/deps.py init
    python tools/deps.py init --s2sdk submodule --protobufs env
    python tools/deps.py update
    python tools/deps.py status

Nothing here needs more than Python 3 and git.
"""

import argparse
import json
import os
import subprocess
import sys
import urllib.request

SDK_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CONFIG_PATH = os.path.join(SDK_ROOT, 'deps.json')
CMAKE_PATH = os.path.join(SDK_ROOT, 'deps.cmake')
VENDOR = os.path.join(SDK_ROOT, 'vendor')

# What the toolkit core is built against in CI (docker/docker-entrypoint.sh
# of the source2toolkit repo). The defaults, all of them overridable.
DEFAULTS = {
    's2sdk': {
        # First set wins; the same list CMakeLists.txt and AMBuildScript read.
        'env': ['S2SDK_CS2', 'S2SDKCS2', 'S2SDK', 'HL2SDK_CS2', 'HL2SDKCS2'],
        'repo': 'https://github.com/alliedmodders/s2sdk.git',
        'branch': 'cs2',
        'dir': 's2sdk',
        # Not cloned by this script: vendor/s2sdk is a submodule of this repo.
        'submodule': True,
        # What has to exist for a path to count as this dependency.
        'probe': os.path.join('public', 'eiface.h'),
    },
    'protobufs': {
        'env': ['CSGO_PROTO'],
        'repo': 'https://github.com/SteamTracking/Protobufs.git',
        'branch': 'master',
        'dir': 'Protobufs',
        # The env var / config path points at the csgo/ folder inside the clone.
        'subdir': 'csgo',
        'probe': 'netmessages.proto',
    },
}

# KHook follows this repository's pin, read from GitHub without cloning it.
KHOOK_TRACK = {
    'repo': 'alliedmodders/metamod-source',
    'branch': 'master',
    'path': 'third_party/khook',
}


DEPS = ('s2sdk', 'protobufs')


# =========================
# Small helpers
# =========================

def env_lookup(dep):
    """(variable, value) of the first environment variable set for dep."""
    for var in DEFAULTS[dep]['env']:
        if os.environ.get(var):
            return var, os.environ[var]
    return DEFAULTS[dep]['env'][0], ''


def env_names(dep):
    return ' / '.join(DEFAULTS[dep]['env'])


def local_mode(dep):
    """What the second init choice is called for dep."""
    return 'submodule' if DEFAULTS[dep].get('submodule') else 'download'


def say(msg):
    print('[deps] ' + msg)


def die(msg):
    print('[deps] error: ' + msg, file=sys.stderr)
    sys.exit(1)


def git(args, cwd, check=True, capture=False):
    cmd = ['git'] + args
    if capture:
        return subprocess.run(cmd, cwd=cwd, check=check, stdout=subprocess.PIPE,
                              stderr=subprocess.STDOUT, text=True).stdout.strip()
    return subprocess.run(cmd, cwd=cwd, check=check).returncode


def is_git_repo(path):
    return os.path.isdir(path) and git(['rev-parse', '--is-inside-work-tree'], path,
                                       check=False, capture=True) == 'true'


def git_head(path):
    return git(['rev-parse', 'HEAD'], path, check=False, capture=True) if is_git_repo(path) else ''


def git_dirty(path):
    return bool(git(['status', '--porcelain', '--untracked-files=no'], path, check=False, capture=True))


def pull(path, label):
    """Fast-forward pull, only when the branch has an upstream to pull from."""
    upstream = git(['rev-parse', '--abbrev-ref', '--symbolic-full-name', '@{u}'], path, check=False, capture=True)
    if not upstream or upstream.startswith('fatal'):
        say('{}: {} has no upstream branch, not pulling'.format(label, path))
        return
    say('{}: pulling {}'.format(label, path))
    git(['pull', '--ff-only', '--quiet'], path, check=False)
    git(['submodule', 'update', '--init', '--recursive', '--quiet'], path, check=False)


def probe(path, dep):
    return bool(path) and os.path.isfile(os.path.join(path, DEFAULTS[dep]['probe']))


def load_config():
    if not os.path.isfile(CONFIG_PATH):
        return None
    with open(CONFIG_PATH, 'r', encoding='utf-8') as f:
        config = json.load(f)
    # Written before s2sdk: the dependency was "hl2sdk", its clone vendor/hl2sdk-cs2.
    if 'hl2sdk' in config and 's2sdk' not in config:
        entry = config.pop('hl2sdk')
        if entry.get('mode') == 'download':
            entry = {'mode': 'submodule'}
        config['s2sdk'] = entry
    return config


def save_config(config):
    with open(CONFIG_PATH, 'w', encoding='utf-8') as f:
        json.dump(config, f, indent=2)
        f.write('\n')
    write_cmake(config)
    say('saved ' + os.path.relpath(CONFIG_PATH, SDK_ROOT) + ' and ' + os.path.relpath(CMAKE_PATH, SDK_ROOT))


def write_cmake(config):
    """CMake 3.18 cannot read JSON, so the paths go into a file it can include.
    Only the downloaded ones and typed paths: a plain env-var choice is what
    CMake does anyway."""
    lines = ['# Generated by tools/deps.py -- do not edit, run `deps.py init` again.']
    for dep, var in (('s2sdk', 'SOURCESDK'), ('protobufs', 'PROTOBUFS')):
        entry = config.get(dep, {})
        if entry.get("mode") in ("download", "submodule") or entry.get("path"):
            lines.append('set({} "{}")'.format(var, resolved_path(dep, entry).replace('\\', '/')))
    with open(CMAKE_PATH, 'w', encoding='utf-8') as f:
        f.write('\n'.join(lines) + '\n')


def resolved_path(dep, entry):
    """Where the dependency is, for the mode saved."""
    d = DEFAULTS[dep]
    if entry['mode'] == 'env':
        return entry.get('path') or env_lookup(dep)[1]
    clone = os.path.join(VENDOR, d['dir'])
    return os.path.join(clone, d['subdir']) if d.get('subdir') else clone


def clone_dir(dep):
    return os.path.join(VENDOR, DEFAULTS[dep]['dir'])


# =========================
# init
# =========================

def ask_choice(dep, env_var, env_path):
    d = DEFAULTS[dep]
    print()
    print('{}:'.format(dep))
    if env_path:
        print('  1) use the checkout {} points at: {}'.format(env_var, env_path))
    else:
        print('  1) use a checkout I already have ({} not set -- you will be asked for the path)'.format(env_names(dep)))
    if d.get('submodule'):
        print('  2) use the vendor/{} submodule of this SDK ({} {})'.format(d['dir'], d['repo'], d['branch']))
    else:
        print('  2) download {} ({}) into vendor/{}'.format(d['repo'], d['branch'], d['dir']))
    while True:
        answer = input('  choice [1/2]: ').strip()
        if answer == '1':
            return 'env'
        if answer == '2':
            return local_mode(dep)


def init(args):
    config = load_config() or {}
    interactive = not (args.s2sdk and args.protobufs)

    for dep in DEPS:
        d = DEFAULTS[dep]
        env_var, env_path = env_lookup(dep)
        mode = getattr(args, dep)
        if mode in ('download', 'submodule'):
            mode = local_mode(dep)
        if not mode:
            if not sys.stdin.isatty():
                die('--{} is required when not run from a terminal'.format(dep))
            mode = ask_choice(dep, env_var, env_path)

        entry = {'mode': mode}
        if mode == 'env':
            path = env_path
            if dep == "protobufs" and env_path and not probe(path, dep) and probe(os.path.join(env_path, "csgo"), dep):
                path = os.path.join(env_path, "csgo")
            if not probe(path, dep):
                if env_path:
                    say('{} = {} does not look like {} ({} missing)'.format(env_var, env_path, dep, d['probe']))
                if interactive and sys.stdin.isatty():
                    path = input('  path to your {} checkout: '.format(dep)).strip()
                    if dep == 'protobufs' and probe(os.path.join(path, 'csgo'), dep):
                        path = os.path.join(path, 'csgo')
                if not probe(path, dep):
                    die('no usable {} checkout at "{}"'.format(dep, path))
            # Only remember an explicit path; the env var is read fresh each time.
            if path != env_path:
                entry['path'] = os.path.abspath(path)
        elif mode == 'download':
            entry['repo'] = getattr(args, dep + '_repo') or d['repo']
            entry['branch'] = getattr(args, dep + '_branch') or d['branch']
        config[dep] = entry

    config['khook'] = {
        'repo': args.metamod_repo or config.get('khook', {}).get('repo', KHOOK_TRACK['repo']),
        'branch': args.metamod_branch or config.get('khook', {}).get('branch', KHOOK_TRACK['branch']),
    }

    save_config(config)
    update(args, config)


# =========================
# update
# =========================

def update_checkout(dep, entry):
    d = DEFAULTS[dep]
    if entry['mode'] == 'env':
        path = resolved_path(dep, entry)
        if dep == 'protobufs' and path.rstrip('/\\').endswith(d['subdir']):
            path = os.path.dirname(path.rstrip('/\\'))
        if not is_git_repo(path):
            say('{}: {} is not a git checkout, left alone'.format(dep, path))
            return
        if git_dirty(path):
            say('{}: {} has local changes, not pulling'.format(dep, path))
            return
        pull(path, dep)
        return

    if entry['mode'] == 'submodule':
        # The commit this SDK pins, not the branch head: the toolkit headers
        # are checked against that one.
        say('{}: vendor/{} -> the commit this SDK pins'.format(dep, DEFAULTS[dep]['dir']))
        git(['submodule', 'update', '--init', '--recursive', 'vendor/' + DEFAULTS[dep]['dir']], SDK_ROOT, check=False)
        return

    path = clone_dir(dep)
    if not os.path.isdir(path):
        os.makedirs(VENDOR, exist_ok=True)
        say('{}: cloning {} ({}) into {}'.format(dep, entry['repo'], entry['branch'], os.path.relpath(path, SDK_ROOT)))
        git(['clone', '--recursive', '--branch', entry['branch'], '--single-branch', entry['repo'], path], SDK_ROOT)
        return

    if git_dirty(path):
        say('{}: {} has local changes, not pulling'.format(dep, os.path.relpath(path, SDK_ROOT)))
        return
    pull(path, dep)


def metamod_khook_sha(track):
    """The commit of third_party/khook in the metamod-source repo, from the
    GitHub contents API -- a submodule entry carries its pinned sha."""
    url = 'https://api.github.com/repos/{}/contents/{}?ref={}'.format(track['repo'], KHOOK_TRACK['path'], track['branch'])
    try:
        with urllib.request.urlopen(urllib.request.Request(url, headers={'User-Agent': 'source2toolkit-sdk-deps'}), timeout=20) as r:
            data = json.load(r)
    except Exception as e:  # noqa: BLE001 -- any failure just means "unknown"
        say('khook: could not read {} ({})'.format(url, e))
        return None
    if data.get('type') != 'submodule':
        say('khook: {} in {} is not a submodule'.format(KHOOK_TRACK['path'], track['repo']))
        return None
    return data.get('sha')


def update_khook(track):
    path = os.path.join(VENDOR, 'khook')
    git(['submodule', 'update', '--init', 'vendor/khook'], SDK_ROOT, check=False)
    if not is_git_repo(path):
        die('vendor/khook is not checked out')

    current = git_head(path)
    wanted = metamod_khook_sha(track)
    if not wanted:
        say('khook: staying on {}'.format(current[:12]))
        return
    if wanted == current:
        say('khook: {} already matches {} {}'.format(current[:12], track['repo'], track['branch']))
        return

    say('khook: {} -> {} (the commit {} {} pins)'.format(current[:12], wanted[:12], track['repo'], track['branch']))
    git(['fetch', '--quiet', 'origin'], path, check=False)
    if git(['cat-file', '-e', wanted + '^{commit}'], path, check=False) != 0:
        git(['fetch', '--quiet', 'origin', wanted], path, check=False)
    git(['checkout', '--quiet', '--detach', wanted], path)
    say('khook: vendor/khook now points elsewhere than this SDK commit; commit the submodule bump if this is the SDK you maintain')


def update_manifests():
    say('manifests: vendor/hl2sdk-manifests -> latest master')
    git(['submodule', 'update', '--init', '--remote', 'vendor/hl2sdk-manifests'], SDK_ROOT, check=False)


def update(args, config=None):
    config = config or load_config()
    if not config:
        die('no deps.json yet -- run `deps.py init` first')

    for dep in DEPS:
        if dep in config:
            update_checkout(dep, config[dep])

    if not getattr(args, 'no_khook', False):
        update_khook(config.get('khook', KHOOK_TRACK))
    if not getattr(args, 'no_manifests', False):
        update_manifests()

    write_cmake(config)
    status(args, config)


# =========================
# status
# =========================

def status(args, config=None):
    config = config or load_config()
    if not config:
        die('no deps.json yet -- run `deps.py init` first')

    print()
    for dep in DEPS:
        entry = config.get(dep)
        if not entry:
            print('  {:<10} not configured'.format(dep))
            continue
        path = resolved_path(dep, entry)
        where = 'from ' + env_lookup(dep)[0] if entry['mode'] == 'env' and 'path' not in entry else entry['mode']
        repo = path
        if dep == 'protobufs' and path.rstrip('/\\').endswith(DEFAULTS[dep]['subdir']):
            repo = os.path.dirname(path.rstrip('/\\'))
        head = git_head(repo)
        ok = 'ok' if probe(path, dep) else 'MISSING ' + DEFAULTS[dep]['probe']
        print('  {:<10} {:<10} {}  [{}] {}'.format(dep, where, path, head[:12] or 'not git', ok))

    khook = git_head(os.path.join(VENDOR, 'khook'))
    track = config.get('khook', KHOOK_TRACK)
    print('  {:<10} {:<10} {}  (tracks {} {})'.format('khook', 'submodule', khook[:12] or 'not checked out', track['repo'], track['branch']))
    manifests = git_head(os.path.join(VENDOR, 'hl2sdk-manifests'))
    print('  {:<10} {:<10} {}'.format('manifests', 'submodule', manifests[:12] or 'not checked out'))

    print()
    for dep in DEPS:
        entry = config.get(dep)
        env_var, env_path = env_lookup(dep)
        if entry and entry['mode'] != 'env' and env_path:
            print('  note: {} is set in your environment but deps.json says "{}" -- '
                  'CMake and AMBuild use vendor/{}.'.format(env_var, entry['mode'], DEFAULTS[dep]['dir']))


# =========================
# main
# =========================

def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest='command', required=True)

    p_init = sub.add_parser('init', help='choose where s2sdk and the Protobufs come from, then update')
    p_init.add_argument('--s2sdk', '--hl2sdk', dest='s2sdk', choices=('env', 'submodule', 'download'),
                        help='"env": the checkout {} points at; "submodule": vendor/s2sdk '
                             '("download" means the same)'.format(env_names('s2sdk')))
    p_init.add_argument('--protobufs', choices=('env', 'download'),
                        help='"env": the checkout CSGO_PROTO points at; "download": clone into vendor/')
    p_init.add_argument('--protobufs-repo', dest='protobufs_repo', help='clone URL for --protobufs download')
    p_init.add_argument('--protobufs-branch', dest='protobufs_branch', help='branch for --protobufs download')
    p_init.add_argument('--metamod-repo', help='GitHub repo whose KHook pin to follow (default {})'.format(KHOOK_TRACK['repo']))
    p_init.add_argument('--metamod-branch', help='branch of that repo (default {})'.format(KHOOK_TRACK['branch']))
    p_init.add_argument('--no-khook', action='store_true', help='leave vendor/khook where it is')
    p_init.add_argument('--no-manifests', action='store_true', help='leave vendor/hl2sdk-manifests where it is')
    p_init.set_defaults(func=init)

    p_update = sub.add_parser('update', help='bring every dependency up to date the way deps.json says')
    p_update.add_argument('--no-khook', action='store_true', help='leave vendor/khook where it is')
    p_update.add_argument('--no-manifests', action='store_true', help='leave vendor/hl2sdk-manifests where it is')
    p_update.set_defaults(func=update)

    p_status = sub.add_parser('status', help='show what is configured and which commits are checked out')
    p_status.set_defaults(func=status)

    args = parser.parse_args()
    args.func(args)


if __name__ == '__main__':
    main()
