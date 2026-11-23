#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_serializer.h"
#include "ast_compile_modules.h"
#include "ast_parse.h"
#include "daScript/misc/job_que.h"
#include "daScript/misc/performance_time.h"

#include <atomic>
#include <thread>

// src/ast/ARCHITECTURE_COMPILE_MODULES.md#compile-dag
namespace das {

    namespace {

        struct ModuleDag {
            vector<vector<int32_t>> deps;
            vector<vector<int32_t>> publicDeps;
            vector<vector<int32_t>> dependents;
            vector<uint64_t>        sourceBytes;

            ModuleDag () = default;

            ModuleDag ( const vector<ModuleInfo> & modules, const FileAccessPtr & access ) {
                das_hash_map<string,int32_t> nodeByFile;
                vector<int32_t> extraDeps;
                for ( auto & mod : modules ) {
                    auto node = int32_t(deps.size());
                    deps.push_back(extraDeps);
                    publicDeps.emplace_back();
                    sourceBytes.push_back(1);
                    if ( FileInfo * fi = access->getFileInfo(mod.fileName) ) {
                        const char * src = nullptr;
                        uint32_t len = 0;
                        fi->getSourceAndLength(src, len);
                        sourceBytes[node] = das::max(len, 1u);
                        string modName;
                        vector<FileInfo *> chain;
                        for ( auto & rec : getAllRequire(fi, modName, chain, access) ) {
                            auto it = nodeByFile.find(access->getModuleInfo(rec.name, mod.fileName).fileName);
                            if ( it == nodeByFile.end() ) continue;
                            auto & nodeDeps = deps[node];
                            if ( find(nodeDeps.begin(), nodeDeps.end(), it->second) == nodeDeps.end() ) nodeDeps.push_back(it->second);
                            auto & pub = publicDeps[node];
                            if ( rec.isPublic && find(pub.begin(), pub.end(), it->second) == pub.end() ) pub.push_back(it->second);
                        }
                    }
                    nodeByFile[mod.fileName] = node;
                    if ( mod.extraDepModule ) extraDeps.push_back(node);
                }
                dependents.resize(deps.size());
                for ( int32_t node = 0; node != int32_t(deps.size()); ++node ) {
                    for ( auto dep : deps[node] ) dependents[dep].push_back(node);
                }
            }

            vector<uint64_t> bottomLevels ( const vector<uint64_t> & weight ) const {
                vector<uint64_t> levels(deps.size(), 0);
                for ( int32_t i = int32_t(deps.size()) - 1; i >= 0; --i ) {
                    uint64_t tail = 0;
                    for ( auto d : dependents[i] ) tail = das::max(tail, levels[d]);
                    levels[i] = weight[i] + tail;
                }
                return levels;
            }

            // src/ast/ARCHITECTURE_COMPILE_MODULES.md#deadlock-prevention
            vector<int32_t> macroUsers ( int32_t macroModule ) const {
                vector<bool> exports(deps.size(), false);
                exports[macroModule] = true;
                for ( size_t i = macroModule + 1; i < deps.size(); ++i ) {
                    for ( auto p : publicDeps[i] ) {
                        if ( exports[p] ) {
                            exports[i] = true;
                            break;
                        }
                    }
                }
                vector<int32_t> users;
                for ( size_t i = macroModule + 1; i < deps.size(); ++i ) {
                    for ( auto d : deps[i] ) {
                        if ( exports[d] ) {
                            users.push_back(int32_t(i));
                            break;
                        }
                    }
                }
                return users;
            }

            struct ReadyQueue {
                const ModuleDag &   dag;
                const vector<bool> & exclusive;
                vector<uint64_t>    priority;
                vector<int32_t>     remaining;
                vector<int32_t>     ready;
                int32_t             completed = 0;
                int32_t             running = 0;
                bool                exclusiveRunning = false;
                ReadyQueue ( const ModuleDag & d, const vector<uint64_t> & estimate, const vector<bool> & excl ) : dag(d), exclusive(excl) {
                    priority = dag.bottomLevels(estimate);
                    remaining.resize(dag.deps.size());
                    for ( size_t i = 0; i != dag.deps.size(); ++i ) {
                        remaining[i] = int32_t(dag.deps[i].size());
                        if ( remaining[i] == 0 ) push(int32_t(i));
                    }
                }
                bool lowerPriority ( int32_t a, int32_t b ) const {
                    if ( priority[a] != priority[b] ) return priority[a] < priority[b];
                    auto fanA = dag.dependents[a].size(), fanB = dag.dependents[b].size();
                    if ( fanA != fanB ) return fanA < fanB;
                    return a > b;
                }
                void push ( int32_t node ) {
                    ready.push_back(node);
                    push_heap(ready.begin(), ready.end(), [this]( int32_t a, int32_t b ) { return lowerPriority(a, b); });
                }
                int32_t tryPop () {
                    if ( ready.empty() || exclusiveRunning ) return -1;
                    // src/ast/ARCHITECTURE_COMPILE_MODULES.md#deadlock-prevention
                    if ( exclusive[ready.front()] ) {
                        if ( running ) return -1;
                        exclusiveRunning = true;
                    }
                    running ++;
                    pop_heap(ready.begin(), ready.end(), [this]( int32_t a, int32_t b ) { return lowerPriority(a, b); });
                    auto node = ready.back();
                    ready.pop_back();
                    return node;
                }
                void complete ( int32_t node ) {
                    completed ++;
                    running --;
                    if ( exclusive[node] ) exclusiveRunning = false;
                    for ( auto d : dag.dependents[node] ) {
                        if ( --remaining[d] == 0 ) push(d);
                    }
                }
                bool done () const { return completed == int32_t(remaining.size()); }
            };

            uint64_t simulatedMakespan ( const vector<uint64_t> & estimate, const vector<uint64_t> & cost,
                    const vector<bool> & exclusive, int32_t workers ) const {
                ReadyQueue que(*this, estimate, exclusive);
                vector<pair<uint64_t,int32_t>> running;
                auto laterFinish = []( const pair<uint64_t,int32_t> & a, const pair<uint64_t,int32_t> & b ) { return a.first > b.first; };
                int32_t idle = workers;
                uint64_t now = 0;
                while ( !que.done() ) {
                    while ( idle ) {
                        auto node = que.tryPop();
                        if ( node < 0 ) break;
                        idle --;
                        running.emplace_back(now + cost[node], node);
                        push_heap(running.begin(), running.end(), laterFinish);
                    }
                    if ( running.empty() ) break;
                    now = running.front().first;
                    while ( !running.empty() && running.front().first == now ) {
                        pop_heap(running.begin(), running.end(), laterFinish);
                        que.complete(running.back().second);
                        running.pop_back();
                        idle ++;
                    }
                }
                return now;
            }

            // Dilworth: the most modules no two of which require each other, directly or not,
            // is the module count less a maximum matching of the reachability relation
            int32_t width () const {
                auto count = int32_t(deps.size());
                vector<vector<bool>> reaches(count, vector<bool>(count, false));
                for ( int32_t node = 0; node != count; ++node ) {
                    for ( auto dep : deps[node] ) {
                        reaches[node][dep] = true;
                        for ( int32_t i = 0; i != count; ++i ) if ( reaches[dep][i] ) reaches[node][i] = true;
                    }
                }
                struct Matching {
                    const vector<vector<bool>> &    reaches;
                    vector<int32_t>                 matchOf;
                    vector<bool>                    seen;
                    bool augment ( int32_t node ) {
                        for ( int32_t dep = 0; dep != int32_t(matchOf.size()); ++dep ) {
                            if ( !reaches[node][dep] || seen[dep] ) continue;
                            seen[dep] = true;
                            if ( matchOf[dep] < 0 || augment(matchOf[dep]) ) {
                                matchOf[dep] = node;
                                return true;
                            }
                        }
                        return false;
                    }
                } matching { reaches, vector<int32_t>(count, -1), {} };
                int32_t matched = 0;
                for ( int32_t node = 0; node != count; ++node ) {
                    matching.seen.assign(count, false);
                    if ( matching.augment(node) ) matched ++;
                }
                return count - matched;
            }

            void log ( const vector<ModuleInfo> & modules, const vector<uint64_t> & usec,
                    ModuleGroup & libGroup, TextWriter & logs ) const {
                auto count = deps.size();
                vector<int32_t> nesting;
                vector<bool> exclusive(count, false);
                for ( size_t i = 0; i != count; ++i ) {
                    auto mod = libGroup.findModule(modules[i].moduleName);
                    if ( mod && mod->macroNestsCompile ) {
                        nesting.push_back(int32_t(i));
                        for ( auto u : macroUsers(int32_t(i)) ) exclusive[u] = true;
                    }
                }
                uint64_t total = 0;
                size_t edges = 0;
                for ( size_t i = 0; i != count; ++i ) {
                    total += usec[i];
                    edges += deps[i].size();
                }
                vector<int32_t> path;
                if ( count ) {
                    auto levels = bottomLevels(usec);
                    int32_t at = int32_t(max_element(levels.begin(), levels.end()) - levels.begin());
                    while ( at >= 0 ) {
                        path.push_back(at);
                        int32_t next = -1;
                        for ( auto d : dependents[at] ) {
                            if ( next == -1 || levels[d] > levels[next] ) next = d;
                        }
                        at = next;
                    }
                }
                uint64_t pathUsec = 0;
                for ( auto n : path ) pathUsec += usec[n];
                logs << "compile DAG: " << count << " modules, " << edges << " edges, total " << (total / 1000000.)
                     << ", critical path " << (pathUsec / 1000000.) << "\n";
                for ( auto m : nesting ) {
                    logs << "\tnesting macro module " << modules[m].moduleName << ", users compile alone:";
                    for ( auto u : macroUsers(m) ) logs << " " << modules[u].moduleName;
                    logs << "\n";
                }
                vector<uint64_t> ones(count, 1);
                for ( int32_t workers : { 2, 4, 8, 16, 32 } ) {
                    auto bound = das::max(pathUsec, (total + workers - 1) / workers);
                    logs << "\t" << workers << " workers: longest path by measured time "
                         << (simulatedMakespan(usec, usec, exclusive, workers) / 1000000.)
                         << ", by source size " << (simulatedMakespan(sourceBytes, usec, exclusive, workers) / 1000000.)
                         << ", by module count " << (simulatedMakespan(ones, usec, exclusive, workers) / 1000000.)
                         << ", lower bound " << (bound / 1000000.) << "\n";
                }
                if ( count ) {
                    auto unlimited = simulatedMakespan(usec, usec, exclusive, int32_t(count));
                    int32_t enough = 1;
                    while ( simulatedMakespan(usec, usec, exclusive, enough) > unlimited ) enough ++;
                    logs << "\twidth: " << width() << " modules can compile at once (by the graph), "
                         << enough << " workers reach the time unlimited workers do (by measured time)\n";
                }
                logs << "critical path:\n";
                for ( auto n : path ) {
                    logs << "\t" << (usec[n] / 1000000.) << " " << modules[n].moduleName << "\n";
                }
                vector<int32_t> heaviest(count);
                for ( size_t i = 0; i != count; ++i ) heaviest[i] = int32_t(i);
                auto shown = das::min(size_t(count), size_t(10));
                partial_sort(heaviest.begin(), heaviest.begin() + shown, heaviest.end(), [&]( int32_t a, int32_t b ) {
                    return usec[a] > usec[b];
                });
                logs << "heaviest modules:\n";
                for ( size_t i = 0; i != shown; ++i ) {
                    logs << "\t" << (usec[heaviest[i]] / 1000000.) << " " << modules[heaviest[i]].moduleName << "\n";
                }
            }
        };

        void nameCompiledModule ( const ProgramPtr & program, const ModuleInfo & mod ) {
            if ( program->thisModule->name.empty() ) {
                program->library.renameModule(program->thisModule.get(),mod.moduleName);
                program->thisModule->wasParsedNameless = true;
            }
            program->thisModule->fileName = mod.fileName;
            program->thisModule->fromExtraDependency = mod.extraDepModule;
        }

        bool registerCompiledModule ( const ProgramPtr & program, const ModuleInfo & mod, const FileAccessPtr & access,
                ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared ) {
            if ( program->failed() || (policies.fail_on_lack_of_aot_export && !aotModuleHasName(program, mod)) ) return false;
            policies.threadlock_context |= program->options.getBoolOption("threadlock_context",false);
            nameCompiledModule(program, mod);
            if ( promoteShared && program->promoteToBuiltin ) {
                if ( !canShareModule(program) ) return false;
                program->thisModule->promoteToBuiltin(access, mod.requireName);
            }
            addNewModules(libGroup, program);
            return true;
        }

        struct InOrderResult {
            ProgramPtr  failed;
            size_t      next = 0;
        };

        // src/ast/ARCHITECTURE_COMPILE_MODULES.md#cache-prefix
        InOrderResult compileModulesInOrder ( const vector<ModuleInfo> & modules, size_t from, const FileAccessPtr & access,
                TextWriter & logs, ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared,
                const ModuleCacheStream & stream, bool untilCutoff, vector<uint64_t> & usec ) {
            InOrderResult result;
            for ( result.next = from; result.next != modules.size(); ++result.next ) {
                if ( untilCutoff && (!stream.read || stream.read->seenNewModule) ) break;
                auto & mod = modules[result.next];
                if ( auto maybeMod = libGroup.findModule(mod.moduleName) ) {
                    maybeMod->fromExtraDependency &= mod.extraDepModule;
                    continue;
                }
                auto moduleT0 = ref_time_ticks();
                auto program = parseDaScriptEx(mod.fileName, mod.moduleName, access, logs, libGroup, true, true, policies, true, stream, nullptr);
                usec[result.next] += get_time_usec(moduleT0);
                if ( !registerCompiledModule(program, mod, access, libGroup, policies, promoteShared) ) {
                    result.failed = program;
                    break;
                }
            }
            return result;
        }

        struct CompiledModule {
            ProgramPtr          program;
            string              logs;
            unique_ptr<gc_root> root;
            vector<Module *>    added;
            CompileTimeTotals   times;
            uint64_t            usec = 0;
            bool                skipped = false;
            bool                deferred = false;
        };

        // src/ast/ARCHITECTURE_COMPILE_MODULES.md#compile-workers
        CompiledModule compileModuleJob ( const ModuleInfo & mod, const ProgramPtr & prepared, const FileAccessPtr & access,
                const CodeOfPolicies & policies, const vector<Module *> & initialModules, const vector<const CompiledModule *> & deps ) {
            CompiledModule result;
            ModuleGroup group;
            for ( auto m : initialModules ) group.addModule(m);
            for ( auto dep : deps ) {
                if ( dep->skipped || dep->deferred || (dep->program && dep->program->failed()) ) {
                    result.skipped = true;
                    group.getModules().clear();
                    return result;
                }
                for ( auto m : dep->added ) group.addModule(m);
            }
            auto t0 = ref_time_ticks();
            result.root = make_unique<gc_root>();
            {
                TextWriter moduleLogs;
                gc_active_scope gcScope(result.root.get());
                result.program = parseDaScriptEx(mod.fileName, mod.moduleName, access, moduleLogs, group, true, true, policies, true,
                    ModuleCacheStream(), prepared, true);
                result.logs = moduleLogs.str();
            }
            auto & program = result.program;
            result.deferred = !program->failed() && program->needMacroModule && !program->thisModule->macroContext;
            if ( !program->failed() && !result.deferred ) {
                nameCompiledModule(program, mod);
                program->thisModule->freeze();
                result.added.push_back(program->thisModule.get());
                program->library.foreach([&](Module * pm) -> bool {
                    if ( !pm->name.empty() && pm->name != "$" && pm != program->thisModule.get() && !group.findModule(pm->name) ) {
                        result.added.push_back(pm);
                    }
                    return true;
                }, "*");
            }
            result.times = group.compileTimes;
            group.getModules().clear();
            result.usec = get_time_usec(t0);
            return result;
        }

        // src/ast/ARCHITECTURE_COMPILE_MODULES.md#compile-workers
        struct FileAccessLockScope {
            FileAccess *    access;
            bool            wasLocked;
            FileAccessLockScope ( FileAccess * acc ) : access(acc), wasLocked(acc->isLocked()) { access->lock(); }
            ~FileAccessLockScope () { if ( !wasLocked ) access->unlock(); }
        };

        // src/ast/ARCHITECTURE_COMPILE_MODULES.md#compile-workers
        struct ModuleJobs {
            const vector<ModuleInfo> &      modules;
            const ModuleDag &               dag;
            const FileAccessPtr &           access;
            CodeOfPolicies                  policies;
            vector<Module *>                initialModules;
            vector<ProgramPtr>              prepared;
            vector<uint64_t>                levels;
            vector<daScriptEnvironment>     envs;
            vector<CompiledModule>          results;
            vector<std::atomic<int32_t>>    remaining;
            std::atomic<int32_t>            finished{0};

            ModuleJobs ( const vector<ModuleInfo> & mods, const ModuleDag & d, const FileAccessPtr & acc, const CodeOfPolicies & pol,
                    ModuleGroup & libGroup )
                : modules(mods), dag(d), access(acc), policies(pol), initialModules(libGroup.getModules()),
                  prepared(mods.size()),
                  envs(mods.size(), *daScriptEnvironment::getBound()), results(mods.size()), remaining(mods.size()) {
                policies.macro_context_collect = false;
                auto count = int32_t(modules.size());
                for ( int32_t node = 0; node != count; ++node ) {
                    envs[node].g_Program = nullptr;
                    if ( libGroup.findModule(modules[node].moduleName) ) continue;
                    prepared[node] = make_smart<Program>();
                    prepared[node]->access = access;
                    int32_t waits = 0;
                    for ( auto dep : dag.deps[node] ) waits += prepared[dep] ? 1 : 0;
                    remaining[node] = waits;
                }
                levels = dag.bottomLevels(dag.sourceBytes);
            }

            void push ( JobQue & que, vector<int32_t> & ready ) {
                stable_sort(ready.begin(), ready.end(), [&]( int32_t a, int32_t b ) { return levels[a] > levels[b]; });
                for ( auto node : ready ) que.push([this, &que, node]() { compileNode(que, node); }, 0, JobPriority::Default);
            }

            void compileNode ( JobQue & que, int32_t node ) {
                auto bound = daScriptEnvironment::getBound();
                daScriptEnvironment::setBound(&envs[node]);
                vector<const CompiledModule *> deps;
                vector<bool> reach(node, false);
                for ( auto dep : dag.deps[node] ) reach[dep] = true;
                for ( int32_t i = node - 1; i >= 0; --i ) {
                    if ( !reach[i] ) continue;
                    deps.push_back(&results[i]);
                    for ( auto dep : dag.deps[i] ) reach[dep] = true;
                }
                results[node] = compileModuleJob(modules[node], prepared[node], access, policies, initialModules, deps);
                daScriptEnvironment::setBound(bound);
                vector<int32_t> ready;
                for ( auto dependent : dag.dependents[node] ) {
                    if ( prepared[dependent] && --remaining[dependent] == 0 ) ready.push_back(dependent);
                }
                push(que, ready);
                finished ++;
            }

            void run ( int32_t threads ) {
                FileAccessLockScope accessLock(access.get());
                JobQue jobs;
                jobs.setWorkerLimit(threads - 1);
                int32_t total = 0;
                vector<int32_t> ready;
                for ( int32_t node = 0; node != int32_t(modules.size()); ++node ) {
                    if ( !prepared[node] ) continue;
                    total ++;
                    if ( remaining[node] == 0 ) ready.push_back(node);
                }
                push(jobs, ready);
                while ( finished < total ) {
                    if ( !jobs.tryRunOneJob() ) std::this_thread::yield();
                }
            }
        };

        ProgramPtr compileModulesOnDag ( const vector<ModuleInfo> & modules, const ModuleDag & dag, const FileAccessPtr & access,
                TextWriter & logs, ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared, int32_t threads,
                vector<uint64_t> & usec, vector<ProgramPtr> & compiled ) {
            ModuleJobs jobs(modules, dag, access, policies, libGroup);
            jobs.run(threads);
            auto & target = *gc_root::gc_get_active_root();
            ProgramPtr failed;
            for ( int32_t node = 0; node != int32_t(modules.size()); ++node ) {
                auto & result = jobs.results[node];
                auto & mod = modules[node];
                usec[node] += result.usec;
                libGroup.compileTimes += result.times;
                if ( !jobs.prepared[node] ) {
                    libGroup.findModule(mod.moduleName)->fromExtraDependency &= mod.extraDepModule;
                    continue;
                }
                if ( failed || result.skipped ) continue;
                logs << result.logs;
                if ( result.root ) {
                    while ( auto gcNode = result.root->gc_first ) gcNode->gc_assign(&target);
                }
                if ( result.deferred ) buildDeferredMacroModule(result.program, libGroup, logs);
                if ( !compiled.empty() ) compiled[node] = result.program;
                if ( !registerCompiledModule(result.program, mod, access, libGroup, policies, promoteShared) ) failed = result.program;
            }
            if ( failed ) return failed;
            return compileModulesInOrder(modules, 0, access, logs, libGroup, policies, promoteShared, ModuleCacheStream(), false,
                usec).failed;
        }

        bool loadedModuleNests ( ModuleGroup & libGroup ) {
            bool nests = false;
            Module::foreach([&]( Module * mod ) { nests |= mod->macroNestsCompile; return !nests; });
            libGroup.foreach([&]( Module * mod ) { nests |= mod->macroNestsCompile; return !nests; }, "*");
            return nests;
        }

    }

    vector<Error> compileModules ( const vector<ModuleInfo> & modules, const FileAccessPtr & access, TextWriter & logs,
            ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared, bool logDag, const ModuleCacheStream & stream ) {
        vector<uint64_t> usec(modules.size(), 0);
        // src/ast/ARCHITECTURE_COMPILE_MODULES.md#cache-prefix
        auto prefix = compileModulesInOrder(modules, 0, access, logs, libGroup, policies, promoteShared, stream, true, usec);
        if ( prefix.failed ) return prefix.failed->errors;
        auto threads = policies.compile_threads == 0 ? JobQue::get_num_threads() : policies.compile_threads;
        bool parallel = threads > 1 && modules.size() - prefix.next > 1 && !loadedModuleNests(libGroup);
        ModuleDag dag;
        if ( parallel || logDag ) dag = ModuleDag(modules, access);
        ProgramPtr failed;
        if ( parallel ) {
            vector<ProgramPtr> compiled(stream.write ? modules.size() : 0);
            failed = compileModulesOnDag(modules, dag, access, logs, libGroup, policies, promoteShared, threads, usec, compiled);
            if ( !failed && stream.write ) {
                for ( auto node = prefix.next; node != modules.size(); ++node ) {
                    if ( !compiled[node] ) continue;
                    recordCompiledModule(stream.write, access, modules[node].fileName, compiled[node],
                        libGroup.findModule(modules[node].moduleName));
                }
            }
        } else {
            failed = compileModulesInOrder(modules, prefix.next, access, logs, libGroup, policies, promoteShared, stream, false,
                usec).failed;
        }
        if ( failed ) return failed->errors;
        if ( logDag ) dag.log(modules, usec, libGroup, logs);
        return {};
    }
}
