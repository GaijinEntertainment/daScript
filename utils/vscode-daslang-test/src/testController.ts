import * as vscode from 'vscode';
import { DaslangConfig } from './config';
import {
    discoverTestFiles,
    discoverTestsInFile,
    getOrCreateFileItem,
    removeFileItem,
} from './testDiscovery';
import { runTests, parseLocation } from './testRunner';

export function createTestController(config: DaslangConfig): vscode.TestController {
    const controller = vscode.tests.createTestController('daslang-dastest', 'daslang Tests');

    // Lazy resolver: root discovers files, file items discover [test] functions
    controller.resolveHandler = async (item) => {
        if (!item) {
            await discoverTestFiles(controller, config.workspaceRoot);
        } else {
            discoverTestsInFile(controller, item);
        }
    };

    // Run profile
    controller.createRunProfile('Run', vscode.TestRunProfileKind.Run, (request, token) => {
        return runHandler(controller, config, request, token);
    });

    return controller;
}

/**
 * Set up a file watcher that keeps the test tree in sync.
 */
export function createFileWatcher(
    controller: vscode.TestController,
    config: DaslangConfig,
): vscode.FileSystemWatcher {
    const watcher = vscode.workspace.createFileSystemWatcher('**/*.das');

    const refresh = (uri: vscode.Uri) => {
        const basename = uri.path.split('/').pop() ?? '';
        if (basename.startsWith('_')) {
            return;
        }
        const fileItem = getOrCreateFileItem(controller, uri);
        discoverTestsInFile(controller, fileItem);
        // Remove file item if it has no test children (no longer a test file)
        if (fileItem.children.size === 0) {
            removeFileItem(controller, uri);
        }
    };

    watcher.onDidChange(refresh);
    watcher.onDidCreate(refresh);
    watcher.onDidDelete((uri) => removeFileItem(controller, uri));

    return watcher;
}

async function runHandler(
    controller: vscode.TestController,
    config: DaslangConfig,
    request: vscode.TestRunRequest,
    token: vscode.CancellationToken,
): Promise<void> {
    const run = controller.createTestRun(request);

    // Collect tests to run, grouped by file
    const fileGroups = new Map<string, vscode.TestItem[]>();
    const testsToRun = request.include ?? gatherAllTests(controller);

    for (const item of testsToRun) {
        if (token.isCancellationRequested) {
            break;
        }

        if (item.children.size > 0 || item.canResolveChildren) {
            // File-level item — run all tests in file
            const filePath = item.uri?.fsPath;
            if (filePath) {
                // Ensure children are discovered
                discoverTestsInFile(controller, item);
                const children: vscode.TestItem[] = [];
                item.children.forEach(child => children.push(child));
                fileGroups.set(filePath, children);
            }
        } else {
            // Function-level item — group by parent file
            const filePath = item.uri?.fsPath;
            if (filePath) {
                const group = fileGroups.get(filePath) ?? [];
                group.push(item);
                fileGroups.set(filePath, group);
            }
        }
    }

    // Execute per file
    for (const [filePath, tests] of fileGroups) {
        if (token.isCancellationRequested) {
            break;
        }

        // Mark tests as started
        for (const test of tests) {
            run.started(test);
        }

        // Determine if we need to filter to specific tests
        const allChildren: vscode.TestItem[] = [];
        const fileItem = controller.items.get(filePath);
        if (fileItem) {
            fileItem.children.forEach(child => allChildren.push(child));
        }
        const runningAll = tests.length >= allChildren.length;
        const testNames = runningAll ? undefined : tests.map(t => t.id.split('/').pop()!);

        const cancellation = { isCancelled: false, kill: undefined as (() => void) | undefined };
        const tokenListener = token.onCancellationRequested(() => {
            cancellation.isCancelled = true;
            cancellation.kill?.();
        });

        try {
            const result = await runTests(config, filePath, testNames, cancellation);

            if (result.timedOut) {
                for (const test of tests) {
                    run.errored(test, new vscode.TestMessage(`Test timed out after ${config.timeout}s`));
                }
                continue;
            }

            if (!result.report) {
                // No JSON report — compilation error or crash
                const message = result.stderr || result.stdout || `Exit code: ${result.exitCode}`;
                for (const test of tests) {
                    run.errored(test, new vscode.TestMessage(message));
                }
                continue;
            }

            // Map JSON results to TestItems
            for (const testResult of result.report.tests) {
                const testItem = tests.find(t => t.id.endsWith('/' + testResult.name));
                if (!testItem) {
                    continue;
                }

                const durationMs = testResult.time / 1000;

                if (testResult.skipped) {
                    run.skipped(testItem);
                } else if (testResult.passed) {
                    run.passed(testItem, durationMs);
                } else {
                    const messageText = testResult.messages.length > 0
                        ? testResult.messages.join('\n')
                        : 'Test failed';
                    const msg = new vscode.TestMessage(messageText);

                    const loc = parseLocation(testResult.location);
                    if (loc && testItem.uri) {
                        msg.location = new vscode.Location(
                            testItem.uri,
                            new vscode.Position(loc.line - 1, 0),
                        );
                    }

                    run.failed(testItem, msg, durationMs);
                }
            }

            // Handle tests not in JSON report (e.g., compilation errors prevented running)
            for (const test of tests) {
                const found = result.report.tests.some(r => test.id.endsWith('/' + r.name));
                if (!found) {
                    run.errored(test, new vscode.TestMessage('Test not found in results'));
                }
            }
        } finally {
            tokenListener.dispose();
        }
    }

    run.end();
}

function gatherAllTests(controller: vscode.TestController): vscode.TestItem[] {
    const items: vscode.TestItem[] = [];
    controller.items.forEach(item => items.push(item));
    return items;
}
