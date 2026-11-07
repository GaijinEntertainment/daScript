#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "daScript/ast/ast_compile_dag.h"
#include "daScript/misc/job_que.h"
#include "daScript/misc/performance_time.h"

#include <future>

// src/ast/ARCHITECTURE_DAG.md#compile-dag
namespace das {

    struct ModuleDag {
        vector<vector<int32_t>> deps;
        vector<vector<int32_t>> publicDeps;
        vector<vector<int32_t>> dependents;
        vector<uint64_t>        sourceBytes;
    };

    static ModuleDag buildModuleDag ( const vector<ModuleInfo> & modules, const FileAccessPtr & access ) {
        ModuleDag dag;
        das_hash_map<string,int32_t> nodeByFile;
        vector<int32_t> extraDeps;
        for ( auto & mod : modules ) {
            auto node = int32_t(dag.deps.size());
            dag.deps.push_back(extraDeps);
            dag.publicDeps.emplace_back();
            dag.sourceBytes.push_back(1);
            if ( FileInfo * fi = access->getFileInfo(mod.fileName) ) {
                const char * src = nullptr;
                uint32_t len = 0;
                fi->getSourceAndLength(src, len);
                dag.sourceBytes[node] = das::max(len, 1u);
                string modName;
                vector<FileInfo *> chain;
                for ( auto & rec : getAllRequire(fi, modName, chain, access) ) {
                    auto it = nodeByFile.find(access->getModuleInfo(rec.name, mod.fileName).fileName);
                    if ( it == nodeByFile.end() ) continue;
                    auto & deps = dag.deps[node];
                    if ( find(deps.begin(), deps.end(), it->second) == deps.end() ) deps.push_back(it->second);
                    auto & pub = dag.publicDeps[node];
                    if ( rec.isPublic && find(pub.begin(), pub.end(), it->second) == pub.end() ) pub.push_back(it->second);
                }
            }
            nodeByFile[mod.fileName] = node;
            if ( mod.extraDepModule ) extraDeps.push_back(node);
        }
        dag.dependents.resize(dag.deps.size());
        for ( int32_t node = 0; node != int32_t(dag.deps.size()); ++node ) {
            for ( auto dep : dag.deps[node] ) dag.dependents[dep].push_back(node);
        }
        return dag;
    }

    static vector<uint64_t> bottomLevels ( const ModuleDag & dag, const vector<uint64_t> & weight ) {
        vector<uint64_t> levels(dag.deps.size(), 0);
        for ( int32_t i = int32_t(dag.deps.size()) - 1; i >= 0; --i ) {
            uint64_t tail = 0;
            for ( auto d : dag.dependents[i] ) tail = das::max(tail, levels[d]);
            levels[i] = weight[i] + tail;
        }
        return levels;
    }

    // src/ast/ARCHITECTURE_DAG.md#deadlock-prevention
    static vector<int32_t> macroUsers ( const ModuleDag & dag, int32_t macroModule ) {
        vector<bool> exports(dag.deps.size(), false);
        exports[macroModule] = true;
        for ( size_t i = macroModule + 1; i < dag.deps.size(); ++i ) {
            for ( auto p : dag.publicDeps[i] ) {
                if ( exports[p] ) {
                    exports[i] = true;
                    break;
                }
            }
        }
        vector<int32_t> users;
        for ( size_t i = macroModule + 1; i < dag.deps.size(); ++i ) {
            for ( auto d : dag.deps[i] ) {
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
            priority = bottomLevels(dag, estimate);
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
            // src/ast/ARCHITECTURE_DAG.md#deadlock-prevention
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

    static uint64_t simulatedMakespan ( const ModuleDag & dag, const vector<uint64_t> & estimate, const vector<uint64_t> & cost,
            const vector<bool> & exclusive, int32_t workers ) {
        ReadyQueue que(dag, estimate, exclusive);
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

    static void logModuleDag ( const vector<ModuleInfo> & modules, const ModuleDag & dag, const vector<uint64_t> & usec,
            ModuleGroup & libGroup, TextWriter & logs ) {
        auto count = dag.deps.size();
        vector<int32_t> nesting;
        vector<bool> exclusive(count, false);
        for ( size_t i = 0; i != count; ++i ) {
            auto mod = libGroup.findModule(modules[i].moduleName);
            if ( mod && mod->macroNestsCompile ) {
                nesting.push_back(int32_t(i));
                for ( auto u : macroUsers(dag, int32_t(i)) ) exclusive[u] = true;
            }
        }
        uint64_t total = 0;
        size_t edges = 0;
        for ( size_t i = 0; i != count; ++i ) {
            total += usec[i];
            edges += dag.deps[i].size();
        }
        vector<int32_t> path;
        if ( count ) {
            auto levels = bottomLevels(dag, usec);
            int32_t at = int32_t(max_element(levels.begin(), levels.end()) - levels.begin());
            while ( at >= 0 ) {
                path.push_back(at);
                int32_t next = -1;
                for ( auto d : dag.dependents[at] ) {
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
            for ( auto u : macroUsers(dag, m) ) logs << " " << modules[u].moduleName;
            logs << "\n";
        }
        vector<uint64_t> ones(count, 1);
        for ( int32_t workers : { 2, 4, 8, 16, 32 } ) {
            auto bound = das::max(pathUsec, (total + workers - 1) / workers);
            logs << "\t" << workers << " workers: longest path by measured time "
                 << (simulatedMakespan(dag, usec, usec, exclusive, workers) / 1000000.)
                 << ", by source size " << (simulatedMakespan(dag, dag.sourceBytes, usec, exclusive, workers) / 1000000.)
                 << ", by module count " << (simulatedMakespan(dag, ones, usec, exclusive, workers) / 1000000.)
                 << ", lower bound " << (bound / 1000000.) << "\n";
        }
        logs << "critical path:\n";
        for ( auto n : path ) {
            logs << "\t" << (usec[n] / 1000000.) << " " << modules[n].moduleName << "\n";
        }
        vector<int32_t> heaviest(count);
        for ( size_t i = 0; i != count; ++i ) heaviest[i] = int32_t(i);
        auto shown = das::min(count, size_t(10));
        partial_sort(heaviest.begin(), heaviest.begin() + shown, heaviest.end(), [&]( int32_t a, int32_t b ) {
            return usec[a] > usec[b];
        });
        logs << "heaviest modules:\n";
        for ( size_t i = 0; i != shown; ++i ) {
            logs << "\t" << (usec[heaviest[i]] / 1000000.) << " " << modules[heaviest[i]].moduleName << "\n";
        }
    }

    static DAS_THREAD_LOCAL(daScriptEnvironment *) tlCompileWorkerMainEnv;

    static void nameCompiledModule ( const ProgramPtr & program, const ModuleInfo & mod ) {
        if ( program->thisModule->name.empty() ) {
            program->library.renameModule(program->thisModule.get(),mod.moduleName);
            program->thisModule->wasParsedNameless = true;
        }
        program->thisModule->fileName = mod.fileName;
        program->thisModule->fromExtraDependency = mod.extraDepModule;
    }

    static bool registerCompiledModule ( const ProgramPtr & program, const ModuleInfo & mod, const FileAccessPtr & access,
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

    static ProgramPtr compileModulesInOrder ( const vector<ModuleInfo> & modules, const FileAccessPtr & access, TextWriter & logs,
            ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared, const ModuleCacheStream & stream,
            vector<uint64_t> & usec ) {
        for ( auto & mod : modules ) {
            auto maybeMod = libGroup.findModule(mod.moduleName);
            if ( maybeMod ) {
                maybeMod->fromExtraDependency &= mod.extraDepModule;
                usec.push_back(0);
                continue;
            }
            auto moduleT0 = ref_time_ticks();
            auto program = parseDaScriptEx(mod.fileName, mod.moduleName, access, logs, libGroup, true, true, policies, true, stream, nullptr);
            usec.push_back(get_time_usec(moduleT0));
            if ( !registerCompiledModule(program, mod, access, libGroup, policies, promoteShared) ) return program;
        }
        return nullptr;
    }

    struct CompiledModule {
        ProgramPtr          program;
        string              logs;
        unique_ptr<gc_root> root;
        vector<Module *>    added;
        uint64_t            usec = 0;
        bool                skipped = false;
        bool                deferred = false;
    };

    // src/ast/ARCHITECTURE_DAG.md#compile-workers
    static ProgramPtr compileModulesOnDag ( const vector<ModuleInfo> & modules, const ModuleDag & dag, const FileAccessPtr & access,
            TextWriter & logs, ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared, int32_t threads,
            vector<uint64_t> & usec ) {
        auto count = int32_t(modules.size());
        vector<Module *> initialModules = libGroup.getModules();
        auto mainEnv = daScriptEnvironment::getBound();
        CodeOfPolicies jobPolicies = policies;
        jobPolicies.macro_context_collect = false;
        vector<ProgramPtr> prepared(count);
        vector<vector<int32_t>> closure(count);
        for ( int32_t node = 0; node != count; ++node ) {
            if ( libGroup.findModule(modules[node].moduleName) ) continue;
            prepared[node] = make_smart<Program>();
            prepared[node]->access = access;
            vector<bool> reach(node, false);
            for ( auto d : dag.deps[node] ) reach[d] = true;
            for ( int32_t i = node - 1; i >= 0; --i ) {
                if ( !reach[i] ) continue;
                closure[node].push_back(i);
                for ( auto d : dag.deps[i] ) reach[d] = true;
            }
        }
        auto levels = bottomLevels(dag, dag.sourceBytes);
        vector<int32_t> order(count);
        for ( int32_t node = 0; node != count; ++node ) order[node] = node;
        stable_sort(order.begin(), order.end(), [&]( int32_t a, int32_t b ) { return levels[a] > levels[b]; });
        vector<std::promise<CompiledModule>> promises(count);
        vector<std::shared_future<CompiledModule>> results(count);
        for ( int32_t node = 0; node != count; ++node ) results[node] = promises[node].get_future().share();
        auto compileNode = [&]( int32_t node ) {
            CompiledModule result;
            if ( !prepared[node] ) return result;
            ModuleGroup group;
            for ( auto m : initialModules ) group.addModule(m);
            for ( auto d : closure[node] ) {
                auto & dep = results[d].get();
                if ( dep.skipped || dep.deferred || (dep.program && dep.program->failed()) ) {
                    result.skipped = true;
                    group.getModules().clear();
                    return result;
                }
                for ( auto m : dep.added ) group.addModule(m);
            }
            auto & mod = modules[node];
            auto t0 = ref_time_ticks();
            result.root = make_unique<gc_root>();
            {
                TextWriter moduleLogs;
                gc_active_scope gcScope(result.root.get());
                result.program = parseDaScriptEx(mod.fileName, mod.moduleName, access, moduleLogs, group, true, true, jobPolicies, true,
                    ModuleCacheStream(), prepared[node], true);
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
            group.getModules().clear();
            result.usec = get_time_usec(t0);
            return result;
        };
        auto workers = das::min(threads, count);
        vector<daScriptEnvironment> envs(workers, *mainEnv);
        for ( auto & env : envs ) {
            env.g_Program = nullptr;
            env.g_compilerLog = nullptr;
            env.g_compilingFileName = nullptr;
            env.g_compilingModuleName = nullptr;
            env.macroTimeTicks = 0;
        }
        std::atomic<int32_t> next{0};
        bool accessWasLocked = access->isLocked();
        access->lock();
        {
            vector<std::thread> pool;
            for ( int32_t w = 0; w != workers; ++w ) {
                pool.emplace_back([&, w]() {
                    daScriptEnvironment::setBound(&envs[w]);
                    *tlCompileWorkerMainEnv = mainEnv;
                    for ( auto i = next++; i < count; i = next++ ) {
                        promises[order[i]].set_value(compileNode(order[i]));
                    }
                    daScriptEnvironment::setBound(nullptr);
                });
            }
            for ( auto & t : pool ) t.join();
        }
        if ( !accessWasLocked ) access->unlock();
        for ( auto & env : envs ) mainEnv->macroTimeTicks += env.macroTimeTicks;
        auto & target = *gc_root::gc_get_active_root();
        ProgramPtr failed;
        for ( int32_t node = 0; node != count; ++node ) {
            auto & result = results[node].get();
            auto & mod = modules[node];
            usec.push_back(result.usec);
            if ( !prepared[node] ) {
                libGroup.findModule(mod.moduleName)->fromExtraDependency &= mod.extraDepModule;
                continue;
            }
            if ( failed || result.skipped ) continue;
            logs << result.logs;
            if ( result.root ) {
                while ( auto gcNode = result.root->gc_first ) gcNode->gc_assign(&target);
            }
            if ( result.deferred ) buildDeferredMacroModule(result.program, libGroup, logs);
            if ( !registerCompiledModule(result.program, mod, access, libGroup, policies, promoteShared) ) failed = result.program;
        }
        if ( failed ) return failed;
        vector<uint64_t> tailUsec;
        failed = compileModulesInOrder(modules, access, logs, libGroup, policies, promoteShared, ModuleCacheStream(), tailUsec);
        for ( size_t node = 0; node != tailUsec.size(); ++node ) usec[node] += tailUsec[node];
        return failed;
    }

    static bool loadedModuleNests ( ModuleGroup & libGroup ) {
        bool nests = false;
        Module::foreach([&]( Module * mod ) { nests |= mod->macroNestsCompile; return !nests; });
        libGroup.foreach([&]( Module * mod ) { nests |= mod->macroNestsCompile; return !nests; }, "*");
        return nests;
    }

    ProgramPtr compileModules ( const vector<ModuleInfo> & modules, const FileAccessPtr & access, TextWriter & logs,
            ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared, bool logDag, const ModuleCacheStream & stream ) {
        auto threads = policies.compile_threads == 0 ? JobQue::get_num_threads() : policies.compile_threads;
        bool parallel = threads > 1 && modules.size() > 1 && !*tlCompileWorkerMainEnv && !stream.read && !stream.write
            && !loadedModuleNests(libGroup);
        if ( !parallel && !logDag ) {
            vector<uint64_t> usec;
            return compileModulesInOrder(modules, access, logs, libGroup, policies, promoteShared, stream, usec);
        }
        auto dag = buildModuleDag(modules, access);
        vector<uint64_t> usec;
        auto failed = parallel
            ? compileModulesOnDag(modules, dag, access, logs, libGroup, policies, promoteShared, threads, usec)
            : compileModulesInOrder(modules, access, logs, libGroup, policies, promoteShared, stream, usec);
        if ( logDag && !failed ) logModuleDag(modules, dag, usec, libGroup, logs);
        return failed;
    }
}
