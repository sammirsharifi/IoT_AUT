from coapthon.client.helperclient import HelperClient

host = "127.0.0.1"
port = 5683
path = "basic"

client = HelperClient(server=(host, port))
response = client.get(path)
print(response)
response1 = client.post(path, "20")
response2 = client.get(path)
print(response2)
response3 = client.post(path, "40")
response4 = client.get(path)
print(response4)
client.stop()
