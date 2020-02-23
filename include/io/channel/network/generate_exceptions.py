exnames = {
    "ConnectionRefused": "ECONNREFUSED",
    "NetUnreachable": "ENETUNREACH",
    "HostUnreachable": "EHOSTUNREACH",
    "TimeOut": "ETIMEDOUT",
    "ConnectionReset": "ECONNRESET",

}
file = open("pexceptions.txt", 'w')
for classname in exnames.keys():
    file.write("struct Socket" + classname + "Exception : public SocketException { explicit Socket" + classname + "Exception(int error_num) : SocketException(error_num) {} };\n")

file.write("\n\n\n\n\n")

for classname in exnames.keys():
    file.write("case " + exnames[classname] + ": throw Socket" + classname + "Exception(__errno);\n")
file.write("default: throw SocketException(__errno);\n")
