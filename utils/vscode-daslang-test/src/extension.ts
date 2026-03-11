import * as vscode from 'vscode';
import { resolveConfig } from './config';
import { createTestController, createFileWatcher } from './testController';

export function activate(context: vscode.ExtensionContext): void {
    const config = resolveConfig();
    if (!config) {
        vscode.window.showWarningMessage(
            'daslang Test Explorer: compiler not found. Set "dascript.compiler" in settings, ' +
            'or open a workspace containing bin/Release/daslang.exe.',
        );
        return;
    }

    const controller = createTestController(config);
    const watcher = createFileWatcher(controller, config);

    context.subscriptions.push(controller);
    context.subscriptions.push(watcher);
}

export function deactivate(): void {
    // Cleanup handled by disposables
}
