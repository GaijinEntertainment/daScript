#include <stdio.h>
#include <uriparser/Uri.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void usage() {
	printf("Usage: uriparse <uri> ...\n");
}


#define RANGE(x) ((x).afterLast-(x).first),((x).first)

int main(int argc, char *argv[]) {
	int retval = 0;
	int i;


	if (argc < 1) {
			usage();
			exit(1);
	}

	for (i=1; i < argc; i++) {
		UriParserStateA state;
		UriUriA uri;
		char ipstr[INET6_ADDRSTRLEN];

		state.uri = &uri;
		printf                          ("uri:          %s\n", argv[i]);
		if (uriParseUriA(&state, argv[i]) != URI_SUCCESS) {
			/* Failure */
			printf                        ("Failure:      %s @ '%.18s' (#%zu)\n", state.errorCode == URI_ERROR_SYNTAX ? "syntax" : state.errorCode == URI_ERROR_MALLOC ? "not enough memory" : "liburiparser bug (please report)", state.errorPos, state.errorPos-argv[i]);
			retval++;
		} else {
			if (uri.scheme.first) 	printf("scheme:       %.*s\n", RANGE(uri.scheme));
			if (uri.userInfo.first) printf("userInfo:     %.*s\n", RANGE(uri.userInfo));
			if (uri.hostText.first) printf("hostText:     %.*s\n", RANGE(uri.hostText));
			if (uri.hostData.ip4) {
				inet_ntop(AF_INET, uri.hostData.ip4->data, ipstr, sizeof ipstr);
															printf("hostData.ip4: %s\n", ipstr);
			}
			if (uri.hostData.ip6) {
				inet_ntop(AF_INET6, uri.hostData.ip6->data, ipstr, sizeof ipstr);
															printf("hostData.ip6: %s\n", ipstr);
			}
			if (uri.portText.first) printf("portText:     %.*s\n", RANGE(uri.portText));
			if (uri.pathHead) {
				UriPathSegmentA *p;
				for (p = uri.pathHead; p; p = p->next) {
															printf(" .. pathSeg:  %.*s\n", RANGE(p->text));
				}
			}
			if (uri.query.first)    printf("query:        %.*s\n", RANGE(uri.query));
			if (uri.fragment.first) printf("fragment:     %.*s\n", RANGE(uri.fragment));
															printf("absolutePath: %s\n", uri.absolutePath == URI_TRUE ? "true" : "false");
															printf("owner:        %s\n", uri.owner == URI_TRUE ? "true" : "false");
		}
		printf("\n");

		uriFreeUriMembersA(&uri);

	}
	return retval;
}

