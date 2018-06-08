#include <map>
#include <string>
#include <utility> 
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"

#include "network/mailbox.h"
#include "network/mailbox_eventloop.h"
#include "network/mailbox_sender.h"
#include "network/mailbox_recver.h"
#include "network/threadsafe_priority_queue.h"
#include "network/mailbox_types.h"


namespace NGAS {
class TestMailbox: public testing::Test {
  public:
    TestMailbox() {
      queue_ = std::make_shared<MailboxEventQueue>();
    }
    ~TestMailbox(){}

  protected:
    void SetUp(){}
    void TearDown(){}
    MailboxEventQueuePtr queue_;
    zmq::context_t zmq_context_;
};

TEST_F(TestMailbox, EventLoopInitDestory) {
  MailboxEventLoop event_loop(queue_);
}

TEST_F(TestMailbox, SendTriggerEventLoop) {
  using namespace std::chrono_literals;

  zmq::context_t zmq_context;
  Mailbox mailbox(queue_);
  MailboxEventLoop event_loop(queue_);
  bool send_triggered = false;
  event_loop.OnSend([&](int worker_id, BinStream*) {
        send_triggered = true;
        EXPECT_EQ(0, worker_id);
      });
  mailbox.Send(0, nullptr);
  std::this_thread::sleep_for(100ms);
  EXPECT_TRUE(send_triggered);
}

TEST_F(TestMailbox, SenderInitDestory) {
  MailboxSender sender({}, &zmq_context_);
}

TEST_F(TestMailbox, RecverInitDestory) {
  MailboxRecver recver("inproc://mailbox-recver", "inproc://mailbox-recver", &zmq_context_, queue_);
}

TEST_F(TestMailbox, SendRecvSimple) {
  using namespace std::chrono_literals;

  MailboxAddressBook addr_book;
  addr_book.AddWorker(0, "inproc://mailbox-recver");
  MailboxEventLoop event_loop(queue_);
  MailboxSender sender(addr_book, &zmq_context_);
  event_loop.OnSend(std::bind(&MailboxSender::Send, &sender, std::placeholders::_1, std::placeholders::_2));
  Mailbox mailbox(queue_);
  bool recv_triggered = false;
  event_loop.SetHandler(MailboxEventType::RecvComm, [&](std::shared_ptr<BinStream> payload) {
        double data;
        *payload >> data;
        EXPECT_EQ(3.14, data);
        recv_triggered = true;
      });
  MailboxRecver recver("inproc://mailbox-recver", "inproc://mailbox-recver", &zmq_context_, queue_);
  BinStream* payload = new BinStream();
  *payload << 3.14;
  sender.Send(0, payload);
  std::this_thread::sleep_for(100ms);
  EXPECT_TRUE(recv_triggered);
}

} // namespace NGAS
