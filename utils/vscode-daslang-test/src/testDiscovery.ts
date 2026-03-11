import * as vscode from 'vscode';
import * as fs from 'fs';

/** Regex to find [test] annotated functions in .das files */
const TEST_ANNOTATION_RE = /^\s*\[test\b[^\]]*\]\s*\r?\n\s*def\s+(\w+)/gm;

/** Quick check: file must require testing_boost to contain [test] functions */
const TESTING_REQUIRE_RE = /require\s+(?:dastest\/)?testing_boost/;

export interface TestFunction {
    name: string;
    line: number; // 0-based line number of the def
}

/**
 * Parse a .das file for [test]-annotated functions.
 * Returns empty array if file doesn't use testing_boost.
 */
export function parseTestFunctions(content: string): TestFunction[] {
    if (!TESTING_REQUIRE_RE.test(content)) {
        return [];
    }
    const results: TestFunction[] = [];
    let match: RegExpExecArray | null;
    TEST_ANNOTATION_RE.lastIndex = 0;
    while ((match = TEST_ANNOTATION_RE.exec(content)) !== null) {
        // Count newlines before the match to get line number
        const upToMatch = content.substring(0, match.index);
        const annotationLine = upToMatch.split('\n').length - 1;
        // The def is on the next line after [test]
        const defLine = annotationLine + 1;
        results.push({ name: match[1], line: defLine });
    }
    return results;
}

/**
 * Check if a file basename should be skipped (underscore-prefixed files are helpers).
 */
function shouldSkipFile(basename: string): boolean {
    return basename.startsWith('_');
}

/**
 * Discover test files and populate the TestItem tree.
 * Creates file-level items; children are resolved lazily.
 */
export async function discoverTestFiles(
    controller: vscode.TestController,
    workspaceRoot: string,
): Promise<void> {
    const pattern = new vscode.RelativePattern(workspaceRoot, '**/*.das');
    const files = await vscode.workspace.findFiles(pattern);

    for (const fileUri of files) {
        const basename = fileUri.path.split('/').pop() ?? '';
        if (shouldSkipFile(basename)) {
            continue;
        }
        try {
            const content = fs.readFileSync(fileUri.fsPath, 'utf8');
            if (TESTING_REQUIRE_RE.test(content)) {
                getOrCreateFileItem(controller, fileUri);
            }
        } catch {
            // Skip unreadable files
        }
    }
}

/**
 * Parse test functions in a file and add them as children of the file item.
 */
export function discoverTestsInFile(
    controller: vscode.TestController,
    fileItem: vscode.TestItem,
): void {
    const uri = fileItem.uri;
    if (!uri) {
        return;
    }
    try {
        const content = fs.readFileSync(uri.fsPath, 'utf8');
        const tests = parseTestFunctions(content);

        // Clear existing children
        fileItem.children.replace([]);

        for (const test of tests) {
            const testId = `${fileItem.id}/${test.name}`;
            const testItem = controller.createTestItem(testId, test.name, uri);
            testItem.range = new vscode.Range(test.line, 0, test.line, 0);
            fileItem.children.add(testItem);
        }
    } catch {
        // Skip unreadable files
    }
}

/**
 * Get or create a file-level TestItem.
 */
export function getOrCreateFileItem(
    controller: vscode.TestController,
    fileUri: vscode.Uri,
): vscode.TestItem {
    const fileId = fileUri.fsPath;
    let fileItem = controller.items.get(fileId);
    if (!fileItem) {
        const label = vscode.workspace.asRelativePath(fileUri);
        fileItem = controller.createTestItem(fileId, label, fileUri);
        fileItem.canResolveChildren = true;
        controller.items.add(fileItem);
    }
    return fileItem;
}

/**
 * Remove a file-level TestItem if it exists.
 */
export function removeFileItem(
    controller: vscode.TestController,
    fileUri: vscode.Uri,
): void {
    controller.items.delete(fileUri.fsPath);
}
