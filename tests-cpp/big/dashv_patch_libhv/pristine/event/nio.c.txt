// Excerpts from pinned libhv 303f50c7, used to test patch anchors.

                } else if (g_ssl_ctx) {
                    ssl_ctx = g_ssl_ctx;
                } else {

            if (io->hostname) {
                hssl_set_sni_hostname(io->ssl, io->hostname);
            }
