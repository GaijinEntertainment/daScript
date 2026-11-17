#!/usr/bin/env python3
"""publish_tts_hf.py's upload step against a recording stand-in for the Hub client: no network, no huggingface_hub.
The staging and the card check need the real set and are the publish's own dry run."""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import publish_tts_hf as pub  # noqa: E402


class Commit:
    oid = "0123abcd"


class FakeApi:
    def __init__(self):
        self.calls = []

    def create_repo(self, repo, **kw):
        self.calls.append(("create_repo", repo, kw))

    def create_branch(self, repo, **kw):
        self.calls.append(("create_branch", repo, kw))

    def upload_folder(self, **kw):
        self.calls.append(("upload_folder", kw))
        return Commit()


class UploadTest(unittest.TestCase):
    def test_a_revision_commits_onto_its_branch_and_makes_it_first(self):
        api = FakeApi()
        self.assertEqual(pub.upload(api, "someone/set", "/work", "a message", "staging"), "0123abcd")
        self.assertEqual([c[0] for c in api.calls], ["create_repo", "create_branch", "upload_folder"])
        self.assertEqual(api.calls[1][2], dict(repo_type="model", branch="staging", exist_ok=True))
        self.assertEqual(api.calls[2][1]["revision"], "staging")

    def test_no_revision_commits_onto_main_and_makes_no_branch(self):
        api = FakeApi()
        pub.upload(api, "someone/set", "/work", "a message", None)
        self.assertEqual([c[0] for c in api.calls], ["create_repo", "upload_folder"])
        self.assertIsNone(api.calls[1][1]["revision"])


if __name__ == "__main__":
    unittest.main()
