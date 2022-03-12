import grpc
from email_pb2 import *
from email_pb2_grpc import *
import asyncio

channel = grpc.insecure_channel('localhost:5000')
stub = EmailStub(channel)

email = "stefan.sa.aleksic@gmail.com"
password = "123"

print(stub.Register(Credentials(email = email, password = password)))

for email in stub.CheckMailbox(Credentials(email=email, password=password)):
    print(email)

for i in range(10):
    print(stub.SendEmail(Content(from_user = email, to_user="unknown@unknown.com", message=f"Hey, what's up {i}?")))