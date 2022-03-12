#include "email.pb.h"
#include "g"
#include <stdio.h>

int main(int argc, char** argv)
{
	grpc::
	CreateChannel("localhost:50051", InsecureChannelCredentials());
}