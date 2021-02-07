#include "autoli/email/email.h"
#include "autoli/email/smtp.h"

void test() {
    autoli::EMail::ptr email = autoli::EMail::Create(
            "user@163.com", "passwd",
            "hello world", "<B>hi xxx</B>hell world", {"564628276@qq.com"});
    autoli::EMailEntity::ptr entity = autoli::EMailEntity::CreateAttach("autoli/autoli.h");
    if(entity) {
        email->addEntity(entity);
    }

    entity = autoli::EMailEntity::CreateAttach("autoli/address.cc");
    if(entity) {
        email->addEntity(entity);
    }

    auto client = autoli::SmtpClient::Create("smtp.163.com", 465, true);
    if(!client) {
        std::cout << "connect smtp.163.com:25 fail" << std::endl;
        return;
    }

    auto result = client->send(email, true);
    std::cout << "result=" << result->result << " msg=" << result->msg << std::endl;
    std::cout << client->getDebugInfo() << std::endl;
    //result = client->send(email, true);
    //std::cout << "result=" << result->result << " msg=" << result->msg << std::endl;
    //std::cout << client->getDebugInfo() << std::endl;
}

int main(int argc, char** argv) {
    autoli::IOManager iom(1);
    iom.schedule(test);
    iom.stop();
    return 0;
}
