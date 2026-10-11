/**
 * Source2Toolkit SDK -- headless schema dump runner.
 *
 * A Metamod plugin that runs Source2SchemaDumper's `dump_schema` command as soon
 * as it is loaded, then ends the process. A dedicated server set up with the
 * binaries and a handful of config files only crashes during startup, while
 * reading game content it does not have -- but Metamod loads its plugins before
 * that, and by then every module's schema is registered. So the dump happens in
 * Load(): AllPluginsLoaded is never reached.
 *
 * The dumper has to be loaded first: list it above this plugin in
 * addons/metamod/metaplugins.ini (see tools/schemadump/dump.sh).
 *
 * SCHEMADUMP_FLAGS overrides the dump_schema arguments (default "all for_cpp").
 */

#include <ISmmPlugin.h>
#include <icvar.h>
#include <tier0/convar.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>

PLUGIN_GLOBALVARS();

class SchemaDumpRunner final : public ISmmPlugin
{
public:
    bool Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late) override
    {
        PLUGIN_SAVEVARS();
        GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);

        ConCommandRef cmd = g_pCVar->FindConCommand("dump_schema");
        if (!cmd.IsValidRef())
        {
            std::fprintf(stderr, "schemadump_runner: dump_schema not found, load the dumper before this plugin\n");
            std::fflush(nullptr);
            _exit(2);
        }

        const char* pszFlags = std::getenv("SCHEMADUMP_FLAGS");
        const std::string command = std::string("dump_schema ") + (pszFlags ? pszFlags : "all for_cpp");

        CCommand args;
        args.Tokenize(command.c_str());
        std::printf("schemadump_runner: %s\n", command.c_str());
        g_pCVar->DispatchConCommand(cmd, CCommandContext(CT_NO_TARGET, CPlayerSlot(0)), args);

        std::printf("schemadump_runner: done\n");
        std::fflush(nullptr);
        _exit(0);
    }

    bool Unload(char* error, size_t maxlen) override { return true; }
    const char* GetAuthor() override { return "Source2Toolkit"; }
    const char* GetName() override { return "SchemaDumpRunner"; }
    const char* GetDescription() override { return "Runs dump_schema at load and exits"; }
    const char* GetURL() override { return "https://github.com/Source2Toolkit/source2toolkit-sdk"; }
    const char* GetLicense() override { return "GPLv3"; }
    const char* GetVersion() override { return "1.0"; }
    const char* GetDate() override { return __DATE__; }
    const char* GetLogTag() override { return "SDRUNNER"; }
};

SchemaDumpRunner g_Runner;
PLUGIN_EXPOSE(SchemaDumpRunner, g_Runner);
