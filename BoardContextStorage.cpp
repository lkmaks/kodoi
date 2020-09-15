#include "BoardContextStorage.h"

BoardContextStorage::BoardContextStorage(Config *config) : config_(config)
{
    nbest_value = config_->default_nbest_value;
}

