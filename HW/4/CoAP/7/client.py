from coapthon.client.helperclient import HelperClient

host = "127.0.0.1"
port = 5683
path = "tank"

client = HelperClient(server=(host, port))
response = client.get(path)
print(response)
client.post(path, "20")
response1= client.get(path)
print(response1)
client.post(path, "40")
response2= client.get(path)
print(response2)
client.post(path, "9")
response2= client.get(path)
print(response2)
for _ in range(5):
    client.post(path,"20")
response4 = client.get(path)
print(response4)
client.stop()
