/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ComposeService_H
#define ComposeService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "twitter_types.h"

namespace twitter {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class ComposeServiceIf {
 public:
  virtual ~ComposeServiceIf() {}
  virtual void getTweet(Tweet& _return, const std::string& user_id, const std::string& tweet_id, const std::string& header) = 0;
};

class ComposeServiceIfFactory {
 public:
  typedef ComposeServiceIf Handler;

  virtual ~ComposeServiceIfFactory() {}

  virtual ComposeServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ComposeServiceIf* /* handler */) = 0;
};

class ComposeServiceIfSingletonFactory : virtual public ComposeServiceIfFactory {
 public:
  ComposeServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<ComposeServiceIf>& iface) : iface_(iface) {}
  virtual ~ComposeServiceIfSingletonFactory() {}

  virtual ComposeServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ComposeServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<ComposeServiceIf> iface_;
};

class ComposeServiceNull : virtual public ComposeServiceIf {
 public:
  virtual ~ComposeServiceNull() {}
  void getTweet(Tweet& /* _return */, const std::string& /* user_id */, const std::string& /* tweet_id */, const std::string& /* header */) {
    return;
  }
};

typedef struct _ComposeService_getTweet_args__isset {
  _ComposeService_getTweet_args__isset() : user_id(false), tweet_id(false), header(false) {}
  bool user_id :1;
  bool tweet_id :1;
  bool header :1;
} _ComposeService_getTweet_args__isset;

class ComposeService_getTweet_args {
 public:

  ComposeService_getTweet_args(const ComposeService_getTweet_args&);
  ComposeService_getTweet_args& operator=(const ComposeService_getTweet_args&);
  ComposeService_getTweet_args() : user_id(), tweet_id(), header() {
  }

  virtual ~ComposeService_getTweet_args() throw();
  std::string user_id;
  std::string tweet_id;
  std::string header;

  _ComposeService_getTweet_args__isset __isset;

  void __set_user_id(const std::string& val);

  void __set_tweet_id(const std::string& val);

  void __set_header(const std::string& val);

  bool operator == (const ComposeService_getTweet_args & rhs) const
  {
    if (!(user_id == rhs.user_id))
      return false;
    if (!(tweet_id == rhs.tweet_id))
      return false;
    if (!(header == rhs.header))
      return false;
    return true;
  }
  bool operator != (const ComposeService_getTweet_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposeService_getTweet_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class ComposeService_getTweet_pargs {
 public:


  virtual ~ComposeService_getTweet_pargs() throw();
  const std::string* user_id;
  const std::string* tweet_id;
  const std::string* header;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposeService_getTweet_result__isset {
  _ComposeService_getTweet_result__isset() : success(false) {}
  bool success :1;
} _ComposeService_getTweet_result__isset;

class ComposeService_getTweet_result {
 public:

  ComposeService_getTweet_result(const ComposeService_getTweet_result&);
  ComposeService_getTweet_result& operator=(const ComposeService_getTweet_result&);
  ComposeService_getTweet_result() {
  }

  virtual ~ComposeService_getTweet_result() throw();
  Tweet success;

  _ComposeService_getTweet_result__isset __isset;

  void __set_success(const Tweet& val);

  bool operator == (const ComposeService_getTweet_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const ComposeService_getTweet_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComposeService_getTweet_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _ComposeService_getTweet_presult__isset {
  _ComposeService_getTweet_presult__isset() : success(false) {}
  bool success :1;
} _ComposeService_getTweet_presult__isset;

class ComposeService_getTweet_presult {
 public:


  virtual ~ComposeService_getTweet_presult() throw();
  Tweet* success;

  _ComposeService_getTweet_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class ComposeServiceClient : virtual public ComposeServiceIf {
 public:
  ComposeServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ComposeServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void getTweet(Tweet& _return, const std::string& user_id, const std::string& tweet_id, const std::string& header);
  void send_getTweet(const std::string& user_id, const std::string& tweet_id, const std::string& header);
  void recv_getTweet(Tweet& _return);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ComposeServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<ComposeServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (ComposeServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_getTweet(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ComposeServiceProcessor(::apache::thrift::stdcxx::shared_ptr<ComposeServiceIf> iface) :
    iface_(iface) {
    processMap_["getTweet"] = &ComposeServiceProcessor::process_getTweet;
  }

  virtual ~ComposeServiceProcessor() {}
};

class ComposeServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ComposeServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< ComposeServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< ComposeServiceIfFactory > handlerFactory_;
};

class ComposeServiceMultiface : virtual public ComposeServiceIf {
 public:
  ComposeServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<ComposeServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ComposeServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<ComposeServiceIf> > ifaces_;
  ComposeServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<ComposeServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void getTweet(Tweet& _return, const std::string& user_id, const std::string& tweet_id, const std::string& header) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getTweet(_return, user_id, tweet_id, header);
    }
    ifaces_[i]->getTweet(_return, user_id, tweet_id, header);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class ComposeServiceConcurrentClient : virtual public ComposeServiceIf {
 public:
  ComposeServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ComposeServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void getTweet(Tweet& _return, const std::string& user_id, const std::string& tweet_id, const std::string& header);
  int32_t send_getTweet(const std::string& user_id, const std::string& tweet_id, const std::string& header);
  void recv_getTweet(Tweet& _return, const int32_t seqid);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

} // namespace

#endif
