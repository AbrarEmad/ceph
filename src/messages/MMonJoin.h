// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2006 Sage Weil <sage@newdream.net>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */

#ifndef CEPH_MMONJOIN_H
#define CEPH_MMONJOIN_H

#include "messages/PaxosServiceMessage.h"

#include <vector>
using std::vector;

class MMonJoin : public MessageInstance<MMonJoin, PaxosServiceMessage> {
public:
  friend factory;

  uuid_d fsid;
  string name;
  entity_addr_t addr;

  MMonJoin() : MessageInstance(MSG_MON_JOIN, 0) {}
  MMonJoin(uuid_d &f, string n, const entity_addr_t& a)
    : MessageInstance(MSG_MON_JOIN, 0),
      fsid(f), name(n), addr(a)
  { }
  
private:
  ~MMonJoin() override {}

public:  
  const char *get_type_name() const override { return "mon_join"; }
  void print(ostream& o) const override {
    o << "mon_join(" << name << " " << addr << ")";
  }
  
  void encode_payload(uint64_t features) override {
    using ceph::encode;
    paxos_encode();
    encode(fsid, payload);
    encode(name, payload);
    encode(addr, payload, features);
  }
  void decode_payload() override {
    auto p = payload.cbegin();
    paxos_decode(p);
    decode(fsid, p);
    decode(name, p);
    decode(addr, p);
  }
};

#endif
