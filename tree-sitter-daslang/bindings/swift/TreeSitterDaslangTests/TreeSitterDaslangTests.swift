import XCTest
import SwiftTreeSitter
import TreeSitterDaslang

final class TreeSitterDaslangTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_daslang())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Daslang grammar")
    }
}
