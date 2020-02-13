/**
 *    SPDX-License-Identifier: AGPL-3.0-only
 *
 *    Copyright (C) 2017-2020 Micron Technology, Inc.
 *
 *    This code is derived from and modifies the mongo-rocks project.
 *
 *    Copyright (C) 2014 MongoDB Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the GNU Affero General Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */
#pragma once

#include "hse.h"

#include <deque>
#include <mutex>
#include <vector>

using namespace std;

extern "C" {

struct hse_kvdb;
struct hse_kvdb_txn;
}

// KVDB interface
namespace hse {

class TxnCache {
public:
    ~TxnCache();

    void set_kvdb(hse_kvdb* kvdb);

    void release();

    hse_kvdb_txn* alloc();

    void free(hse_kvdb_txn* txn);

private:
    hse_kvdb* _kvdb{0};
};

extern TxnCache* g_txn_cache;

class ClientTxn {
public:
    ClientTxn(struct hse_kvdb* kvdb);

    virtual ~ClientTxn();

    Status begin();

    Status commit();

    Status abort();

    struct hse_kvdb_txn* get_kvdb_txn() {
        return _txn;
    }

private:
    static TxnCache _txn_cache;
    struct hse_kvdb* _kvdb;
    struct hse_kvdb_txn* _txn;
};
}
