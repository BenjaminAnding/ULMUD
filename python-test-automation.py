import telnetlib
import glob
import time

tests = glob.iglob("tests/*")
for test in tests:
	lineprompts = []
	scriptlines = []
	lineresponses = []
	with open(test, 'r') as script:
		for line in script:
			lineprompts.append(line.split(':')[0])	
			scriptlines.append(line.split(':')[1])
			lineresponses.append(line.split(':')[2].strip())
	#print(lineprompts)
	#print(scriptlines)
	#print(lineresponses)
	tn = telnetlib.Telnet()
	tn.open("localhost", "4000", 10)
	for i in range(len(scriptlines)):
		#print(lineprompts[i])
		print("Trying: "+scriptlines[i])
		#print(lineresponses[i])
		response = tn.read_until(lineprompts[i].encode("ascii"))
		tn.write((scriptlines[i]+"\n").encode("ascii"))
		if i > 0:
			if lineresponses[i-1] in response.decode("ascii"):
				print("Passed")
			else:
				print("Failed")



#tn = telnetlib.Telnet()
#tn.open("localhost", "4000", 100)
#tn.read_until("Enter your name, or 'new' to create a new character ...".encode("ascii"))
#tn.write("Ben\n".encode("ascii"))
#tn.read_until("Enter your password ...".encode("ascii"))
#tn.write("ben\n".encode("ascii"))
#print(tn.read_until("\n>".encode("ascii")))
#tn.write("say Yo\n".encode("ascii"))
#print(tn.read_until("\n>".encode("ascii")))
#tn.write("quit\n".encode("ascii"))
#print(tn.read_until("\n>".encode("ascii")))
#print(tn.read_all())


