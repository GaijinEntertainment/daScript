#include "daScript/misc/platform.h"

#include "daScript/ast/ast.h"
#include "ast_compile_modules.h"
#include "ast_parse.h"
#include "daScript/misc/performance_time.h"

// src/ast/ARCHITECTURE_COMPILE_MODULES.md#compile-dag
namespace das {

    namespace {

        struct ModuleDag {
            vector<vector<int32_t>> deps;
            vector<vector<int32_t>> dependents;
            vector<uint64_t>        sourceBytes;

            ModuleDag () = default;

            ModuleDag ( const vector<ModuleInfo> & modules, const FileAccessPtr & access ) {
                das_hash_map<string,int32_t> nodeByFile;
                vector<int32_t> extraDeps;
                for ( auto & mod : modules ) {
                    auto node = int32_t(deps.size());
                    deps.push_back(extraDeps);
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

            struct ReadyQueue {
                const ModuleDag &   dag;
                vector<uint64_t>    priority;
                vector<int32_t>     remaining;
                vector<int32_t>     ready;
                int32_t             completed = 0;
                ReadyQueue ( const ModuleDag & d, const vector<uint64_t> & estimate ) : dag(d) {
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
                    if ( ready.empty() ) return -1;
                    pop_heap(ready.begin(), ready.end(), [this]( int32_t a, int32_t b ) { return lowerPriority(a, b); });
                    auto node = ready.back();
                    ready.pop_back();
                    return node;
                }
                void complete ( int32_t node ) {
                    completed ++;
                    for ( auto d : dag.dependents[node] ) {
                        if ( --remaining[d] == 0 ) push(d);
                    }
                }
                bool done () const { return completed == int32_t(remaining.size()); }
            };

            uint64_t simulatedMakespan ( const vector<uint64_t> & estimate,
                    const vector<uint64_t> & cost, int32_t workers ) const {
                ReadyQueue que(*this, estimate);
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

            void log ( const vector<ModuleInfo> & modules,
                    const vector<uint64_t> & usec, TextWriter & logs ) const {
                auto count = deps.size();
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
                vector<uint64_t> ones(count, 1);
                for ( int32_t workers : { 2, 4, 8, 16, 32 } ) {
                    auto bound = das::max(pathUsec, (total + workers - 1) / workers);
                    logs << "\t" << workers << " workers: longest path by measured time "
                         << (simulatedMakespan(usec, usec, workers) / 1000000.)
                         << ", by source size " << (simulatedMakespan(sourceBytes, usec, workers) / 1000000.)
                         << ", by module count " << (simulatedMakespan(ones, usec, workers) / 1000000.)
                         << ", lower bound " << (bound / 1000000.) << "\n";
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

        ProgramPtr compileModulesInOrder ( const vector<ModuleInfo> & modules, const FileAccessPtr & access, TextWriter & logs,
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
                auto program = parseDaScriptEx(mod.fileName, mod.moduleName, access, logs, libGroup, true, true, policies, true, stream);
                usec.push_back(get_time_usec(moduleT0));
                if ( !registerCompiledModule(program, mod, access, libGroup, policies, promoteShared) ) return program;
            }
            return nullptr;
        }

    }

    vector<Error> compileModules ( const vector<ModuleInfo> & modules, const FileAccessPtr & access, TextWriter & logs,
            ModuleGroup & libGroup, CodeOfPolicies & policies, bool promoteShared, bool logDag, const ModuleCacheStream & stream ) {
        vector<uint64_t> usec;
        auto failed = compileModulesInOrder(modules, access, logs, libGroup, policies, promoteShared, stream, usec);
        if ( failed ) return failed->errors;
        if ( logDag ) ModuleDag(modules, access).log(modules, usec, logs);
        return {};
    }
}
