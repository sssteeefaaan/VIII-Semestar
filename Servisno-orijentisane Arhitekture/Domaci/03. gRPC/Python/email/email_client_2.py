import grpc
from email_pb2 import *
from email_pb2_grpc import *
import asyncio

channel = grpc.insecure_channel('localhost:5000')
stub = EmailStub(channel)

email = "unknown@unknown.com"
password = "uknown123"

print(stub.Register(Credentials(email = email, password = password)))

for email in stub.CheckMailbox(Credentials(email=email, password=password)):
    print(email)