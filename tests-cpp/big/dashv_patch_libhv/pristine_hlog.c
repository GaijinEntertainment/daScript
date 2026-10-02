static void logger_init(logger_t* logger) {
    logger->enable_fsync = 1;
    logger_set_file(logger, DEFAULT_LOG_FILE);
    logger->last_logfile_ts = 0;
    logger->can_write_cnt = -1;
    hmutex_init(&logger->mutex_);
}

void logger_set_file(logger_t* logger, const char* filepath) {
    strncpy(logger->filepath, filepath, sizeof(logger->filepath) - 1);
    // remove suffix .log
    char* suffix = strrchr(logger->filepath, '.');
    if (suffix && strcmp(suffix, ".log") == 0) {
        *suffix = '\0';
    }
}

void logger_enable_fsync(logger_t* logger, int on) {
    logger->enable_fsync = on;
}
