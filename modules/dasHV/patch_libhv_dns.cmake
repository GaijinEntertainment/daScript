# A lookup that names no nameserver of its own is answered by the operating system resolver, not by
# libhv's own: modules/dasHV/ARCHITECTURE.md#name-resolution
das_hv_patch_begin("event/hdns.c")
das_hv_hunk([=[
    // 2) hosts + cache lookups (need config loaded)
]=] [=[
    if (!q->opt.nameserver) {
        struct addrinfo hints;
        struct addrinfo* found = NULL;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = q->opt.family == HDNS_QUERY_A ? AF_INET : q->opt.family == HDNS_QUERY_AAAA ? AF_INET6 : AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo(host, NULL, &hints, &found) == 0) {
            for (struct addrinfo* ai = found; ai && q->naddrs < HDNS_MAX_ADDRS; ai = ai->ai_next) {
                if ((ai->ai_family != AF_INET && ai->ai_family != AF_INET6) || ai->ai_addrlen > sizeof(num)) continue;
                memset(&num, 0, sizeof(num));
                memcpy(&num, ai->ai_addr, ai->ai_addrlen);
                q->addrs[q->naddrs++] = num;
            }
            freeaddrinfo(found);
        }
        q->opt.use_cache = 0;
        hdns__defer_complete(q);
        return q;
    }

    // 2) hosts + cache lookups (need config loaded)
]=])
das_hv_patch_end()
