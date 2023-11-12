#include <UnitTest++/UnitTest++.h>
#include "ServerInteraction.h"

using namespace std;

struct Cons_fix {
	ServerInteraction * p;
	Cons_fix() {
		p = new ServerInteraction();
	}
	~Cons_fix() {
		delete p;
	}
};

SUITE(interactWithServer)
{
    TEST_FIXTURE(Cons_fix, SuccessfulInteractionWithServer){
		p->SetServerAddress(2130706433);
        p->SetServerPort(33333);
        p->SetVectorDataFile("/home/user/VSCode/TIMP/projects1/output.bin");
        p->SetResultFile("/home/user/VSCode/TIMP/projects1/resultat.txt");
        p->SetClientAuthFile("/home/user/VSCode/TIMP/projects1/config/vclient.conf");
        p->perform_server_interaction();
        CHECK(true);	
	}
	

    TEST_FIXTURE(Cons_fix, InvalidIPAddressFormat){
		p->SetServerAddress(2130706433);
        p->SetServerPort(33333);
        p->SetVectorDataFile("/home/user/VSCode/TIMP/projects1/output.bin");
        p->SetResultFile("/home/user/VSCode/TIMP/projects1/resultat.txt");
        p->SetClientAuthFile("/home/user/VSCode/TIMP/projects1/config/vclient.conf");
        CHECK_THROW(p->perform_server_interaction(), ExceptionManager);
	}

	TEST_FIXTURE(Cons_fix, InvalidPort){
		p->SetServerAddress(2130706433);
        p->SetServerPort(444444);
        p->SetVectorDataFile("/home/user/VSCode/TIMP/projects1/output.bin");
        p->SetResultFile("/home/user/VSCode/TIMP/projects1/resultat.txt");
        p->SetClientAuthFile("/home/user/VSCode/TIMP/projects1/config/vclient.conf");
        CHECK_THROW(p->perform_server_interaction(), ExceptionManager);
	}

    TEST_FIXTURE(Cons_fix, ErrorOpeningLoginAndPasswordFile){
		p->SetServerAddress(2130706433);
        p->SetServerPort(33333);
        p->SetVectorDataFile("/home/user/VSCode/TIMP/projects1/output.bin");
        p->SetResultFile("/home/user/VSCode/TIMP/projects1/resultat.txt");
        p->SetClientAuthFile("/home/user/VSCode/TIMP/projects1/config/.@34!/vclient.conf");
        CHECK_THROW(p->perform_server_interaction(), ExceptionManager);
	}

    TEST_FIXTURE(Cons_fix, UserIdentificationError){
		p->SetServerAddress(2130706433);
        p->SetServerPort(33333);
        p->SetVectorDataFile("/home/user/VSCode/TIMP/projects1/output.bin");
        p->SetResultFile("/home/user/VSCode/TIMP/projects1/resultat.txt");
        p->SetClientAuthFile("/home/user/VSCode/TIMP/projects1/config/UserIdentificationError.conf");
        CHECK_THROW(p->perform_server_interaction(), ExceptionManager);
	}

    TEST_FIXTURE(Cons_fix, AuthenticationError){
		p->SetServerAddress(2130706433);
        p->SetServerPort(33333);
        p->SetVectorDataFile("/home/user/VSCode/TIMP/projects1/output.bin");
        p->SetResultFile("/home/user/VSCode/TIMP/projects1/resultat.txt");
        p->SetClientAuthFile("/home/user/VSCode/TIMP/projects1/config/AuthenticationError.conf");
        CHECK_THROW(p->perform_server_interaction(), ExceptionManager);
	}

    TEST_FIXTURE(Cons_fix, ErrorOpeningVectorFile){
		p->SetServerAddress(2130706433);
        p->SetServerPort(33333);
        p->SetVectorDataFile("/home/user/VSCode/TIMP/projects1/!@/output.bin");
        p->SetResultFile("/home/user/VSCode/TIMP/projects1/resultat.txt");
        p->SetClientAuthFile("/home/user/VSCode/TIMP/projects1/config/vclient.conf");
        CHECK_THROW(p->perform_server_interaction(), ExceptionManager);
	}

    TEST_FIXTURE(Cons_fix, ErrorReadingFromVectorFile){
		p->SetServerAddress(2130706433);
        p->SetServerPort(33333);
        p->SetVectorDataFile("/home/user/VSCode/TIMP/projects1/ErrorReadingFromVectorFile.bin");
        p->SetResultFile("/home/user/VSCode/TIMP/projects1/resultat.txt");
        p->SetClientAuthFile("/home/user/VSCode/TIMP/projects1/config/vclient.conf");
        CHECK_THROW(p->perform_server_interaction(), ExceptionManager);
	}

    TEST_FIXTURE(Cons_fix, ErrorOpeningFileForSummationResults){
		p->SetServerAddress(2130706433);
        p->SetServerPort(33333);
        p->SetVectorDataFile("/home/user/VSCode/TIMP/projects1/output.bin");
        p->SetResultFile("/home/user/VSCode/TIMP/projects1/!@/resultat.txt");
        p->SetClientAuthFile("/home/user/VSCode/TIMP/projects1/config/vclient.conf");
        CHECK_THROW(p->perform_server_interaction(), ExceptionManager);
	}
}

int main(int argc, char **argv)
{
	return UnitTest::RunAllTests();
}
