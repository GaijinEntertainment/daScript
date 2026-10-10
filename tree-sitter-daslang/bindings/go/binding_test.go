package tree_sitter_daslang_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_daslang "github.com/GaijinEntertainment/tree-sitter-daslang/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	parser := tree_sitter.NewParser()
	defer parser.Close()

	if err := parser.SetLanguage(tree_sitter.NewLanguage(tree_sitter_daslang.Language())); err != nil {
		t.Errorf("Error loading Daslang grammar: %v", err)
	}
}
