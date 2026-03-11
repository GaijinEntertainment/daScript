import * as vscode from 'vscode';
import * as path from 'path';
import * as fs from 'fs';

export interface DaslangConfig {
    compilerPath: string;
    dastestPath: string;
    workspaceRoot: string;
    timeout: number;
}

function findFile(...candidates: string[]): string | undefined {
    for (const c of candidates) {
        if (c && fs.existsSync(c)) {
            return c;
        }
    }
    return undefined;
}

export function resolveConfig(): DaslangConfig | undefined {
    const folders = vscode.workspace.workspaceFolders;
    if (!folders || folders.length === 0) {
        return undefined;
    }
    const workspaceRoot = folders[0].uri.fsPath;
    const cfg = vscode.workspace.getConfiguration('dascript');
    const timeout = cfg.get<number>('testTimeout', 120);

    // Resolve compiler path
    const compilerSetting = cfg.get<string>('compiler', '');
    const compilerPath = findFile(
        compilerSetting,
        path.join(workspaceRoot, 'bin', 'Release', 'daslang.exe'),
        path.join(workspaceRoot, 'bin', 'Debug', 'daslang.exe'),
    );
    if (!compilerPath) {
        return undefined;
    }

    // Resolve dastest path
    const dastestSetting = cfg.get<string>('dastestPath', '');
    const dastestPath = findFile(
        dastestSetting,
        path.join(workspaceRoot, 'dastest', 'dastest.das'),
    );
    if (!dastestPath) {
        return undefined;
    }

    return { compilerPath, dastestPath, workspaceRoot, timeout };
}
