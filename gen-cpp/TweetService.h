/**
 * Autogenerated by Thrift Compiler (0.11.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef TweetService_H
#define TweetService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "twitter_types.h"

namespace twitter {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class TweetServiceIf {
 public:
  virtual ~TweetServiceIf() {}
  virtual void getTweet_(Tweet_& _return, const std::string& tweet_id) = 0;
};

class TweetServiceIfFactory {
 public:
  typedef TweetServiceIf Handler;

  virtual ~TweetServiceIfFactory() {}

  virtual TweetServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(TweetServiceIf* /* handler */) = 0;
};

class TweetServiceIfSingletonFactory : virtual public TweetServiceIfFactory {
 public:
  TweetServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<TweetServiceIf>& iface) : iface_(iface) {}
  virtual ~TweetServiceIfSingletonFactory() {}

  virtual TweetServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(TweetServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<TweetServiceIf> iface_;
};

class TweetServiceNull : virtual public TweetServiceIf {
 public:
  virtual ~TweetServiceNull() {}
  void getTweet_(Tweet_& /* _return */, const std::string& /* tweet_id */) {
    return;
  }
};

typedef struct _TweetService_getTweet__args__isset {
  _TweetService_getTweet__args__isset() : tweet_id(false) {}
  bool tweet_id :1;
} _TweetService_getTweet__args__isset;

class TweetService_getTweet__args {
 public:

  TweetService_getTweet__args(const TweetService_getTweet__args&);
  TweetService_getTweet__args& operator=(const TweetService_getTweet__args&);
  TweetService_getTweet__args() : tweet_id() {
  }

  virtual ~TweetService_getTweet__args() throw();
  std::string tweet_id;

  _TweetService_getTweet__args__isset __isset;

  void __set_tweet_id(const std::string& val);

  bool operator == (const TweetService_getTweet__args & rhs) const
  {
    if (!(tweet_id == rhs.tweet_id))
      return false;
    return true;
  }
  bool operator != (const TweetService_getTweet__args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TweetService_getTweet__args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class TweetService_getTweet__pargs {
 public:


  virtual ~TweetService_getTweet__pargs() throw();
  const std::string* tweet_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TweetService_getTweet__result__isset {
  _TweetService_getTweet__result__isset() : success(false) {}
  bool success :1;
} _TweetService_getTweet__result__isset;

class TweetService_getTweet__result {
 public:

  TweetService_getTweet__result(const TweetService_getTweet__result&);
  TweetService_getTweet__result& operator=(const TweetService_getTweet__result&);
  TweetService_getTweet__result() {
  }

  virtual ~TweetService_getTweet__result() throw();
  Tweet_ success;

  _TweetService_getTweet__result__isset __isset;

  void __set_success(const Tweet_& val);

  bool operator == (const TweetService_getTweet__result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const TweetService_getTweet__result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const TweetService_getTweet__result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _TweetService_getTweet__presult__isset {
  _TweetService_getTweet__presult__isset() : success(false) {}
  bool success :1;
} _TweetService_getTweet__presult__isset;

class TweetService_getTweet__presult {
 public:


  virtual ~TweetService_getTweet__presult() throw();
  Tweet_* success;

  _TweetService_getTweet__presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class TweetServiceClient : virtual public TweetServiceIf {
 public:
  TweetServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  TweetServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void getTweet_(Tweet_& _return, const std::string& tweet_id);
  void send_getTweet_(const std::string& tweet_id);
  void recv_getTweet_(Tweet_& _return);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class TweetServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<TweetServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (TweetServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_getTweet_(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  TweetServiceProcessor(::apache::thrift::stdcxx::shared_ptr<TweetServiceIf> iface) :
    iface_(iface) {
    processMap_["getTweet_"] = &TweetServiceProcessor::process_getTweet_;
  }

  virtual ~TweetServiceProcessor() {}
};

class TweetServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  TweetServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< TweetServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< TweetServiceIfFactory > handlerFactory_;
};

class TweetServiceMultiface : virtual public TweetServiceIf {
 public:
  TweetServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<TweetServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~TweetServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<TweetServiceIf> > ifaces_;
  TweetServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<TweetServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void getTweet_(Tweet_& _return, const std::string& tweet_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->getTweet_(_return, tweet_id);
    }
    ifaces_[i]->getTweet_(_return, tweet_id);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class TweetServiceConcurrentClient : virtual public TweetServiceIf {
 public:
  TweetServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  TweetServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  void getTweet_(Tweet_& _return, const std::string& tweet_id);
  int32_t send_getTweet_(const std::string& tweet_id);
  void recv_getTweet_(Tweet_& _return, const int32_t seqid);
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