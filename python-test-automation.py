import telnetlib

tn = telnetlib.Telnet()
tn.open("localhost", "4000", 100)
tn.read_until("Enter your name, or 'new' to create a new character ...".encode("ascii"))
tn.write("Ben\n".encode("ascii"))
tn.read_until("Enter your password ...".encode("ascii"))
tn.write("ben\n".encode("ascii"))
print(tn.read_until("\n>".encode("ascii")))
tn.write("say Yo\n".encode("ascii"))
print(tn.read_until("\n>".encode("ascii")))
tn.write("quit\n".encode("ascii"))
print(tn.read_until("\n>".encode("ascii")))
print(tn.read_all())

