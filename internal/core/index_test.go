package core

import (
	"testing"
)

func TestBuildInvertedIndex(t *testing.T) {
	sampleDocs := []string{
		"GNU Debugger is a tool for debugging software.",
		"CMake is a build tool.",
	}
	idx := BuildInvertedIndex(sampleDocs)

	if len(idx) == 0 {
		t.Fatal("expected non-empty inverted index")
	}

	debuggerPostings, ok := idx["debugger"]
	if !ok {
		t.Fatal("expected term 'debugger' in inverted index")
	}

	if debuggerPostings.TotalFrequency <= 0 {
		t.Errorf("expected total frequency > 0, got %d", debuggerPostings.TotalFrequency)
	}

	if len(debuggerPostings.Entries) == 0 {
		t.Errorf("expected entries for 'debugger'")
	}
}
