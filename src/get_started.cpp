#include <iostream>

#include "rocksdb/db.h"

std::optional<rocksdb::DB*> open(const std::string& dir) {
	rocksdb::DB* db;
	rocksdb::Options options;
	options.create_if_missing = true;
	auto status = rocksdb::DB::Open(options, dir, &db);
	if (status.ok()) {
		return db;
	}
	std::cerr << status.ToString() << std::endl;
	return std::nullopt;
}

void close(rocksdb::DB* db) {
	delete db;
}

std::optional<std::string> get(rocksdb::DB* db, const std::string& key) {
	std::string value;
	auto status = db->Get(rocksdb::ReadOptions(), key, &value);

	if (status.ok()) {
		return value;
	}
	return std::nullopt;
}

bool put(rocksdb::DB* db, const std::string& key, const std::string& value) {
	auto status = db->Put(rocksdb::WriteOptions(), key, value);
	return status.ok();
}

int main(int argc, char const* argv[]) {
	std::string dir = "./data";
	auto dbOpt = open(dir);
	if (!dbOpt) {
		std::cerr << "Cannot open database." << std::endl;
		return 1;
	}
	auto db = dbOpt.value();

	bool init = false;
	auto versionOpt = get(db, "version");
	if (!versionOpt) {
		put(db, "version", "1.0");
		init = true;
	} else {
		std::cout << versionOpt.value() << std::endl;
	}

	if (init) {
		put(db, "key1", "value1");
		put(db, "key2", "value2");
		put(db, "key3", "value3");
	} else {
		auto valOpt = get(db, "key1");
		if (!valOpt) {
			std::cerr << "Cannot get key1." << std::endl;
		} else {
			std::cout << "key1=" << valOpt.value() << std::endl;
		}
	}

	close(db);
	return 0;
}
