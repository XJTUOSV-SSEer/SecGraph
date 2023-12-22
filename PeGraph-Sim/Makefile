SUBDIRS = OXT pbcwrapper Util
SRCS = $(wildcard $(addsuffix /*.cc,$(SUBDIRS)))
OBJS = $(wildcard $(addsuffix /*.o,$(SUBDIRS)))

CXXFLAGS=-O2 -Wall -I/usr/include -I/usr/include/openssl

TARGETS = libPBC.a OXTmain

all: $(TARGETS)

COMMON_OBJS=pbcwrapper/Pairing.o pbcwrapper/G.o pbcwrapper/G1.o pbcwrapper/G2.o pbcwrapper/GT.o pbcwrapper/Zr.o pbcwrapper/PPPairing.o 


libPBC.a: $(COMMON_OBJS)
	ar rcs $@ $^

# OXTmain: OXT/OXTmain.o OXT/OXTProtocol.o Util/Utils.o libPBC.a
# 	g++ -g $(CXXFLAGS) -stati -o $@ $^ -lssl -lcrypto -lpbc -lgmp -lpthread -ldl

OXTmain: OXT/OXTmain.o OXT/OXTProtocol.o Util/Utils.o pbcwrapper/Pairing.o pbcwrapper/G.o pbcwrapper/G1.o pbcwrapper/G2.o pbcwrapper/GT.o pbcwrapper/Zr.o pbcwrapper/PPPairing.o 
	g++ -g $(CXXFLAGS) -o $@ $^ -lssl -lcrypto -lpbc -lgmp -lpthread -ldl

clean:
	-rm -f $(OBJS)

veryclean: clean
	-rm -f $(TARGETS)

depend:
	makedepend -Y -- $(CXXFLAGS) -- $(SRCS) 2>/dev/null


# DO NOT DELETE

pbcwrapper/G1.o: pbcwrapper/G1.h pbcwrapper/G.h pbcwrapper/Pairing.h pbcwrapper/Zr.h pbcwrapper/PBCExceptions.h
pbcwrapper/G2.o: pbcwrapper/G2.h pbcwrapper/G.h pbcwrapper/Pairing.h pbcwrapper/Zr.h pbcwrapper/PBCExceptions.h
pbcwrapper/G.o: pbcwrapper/G.h pbcwrapper/Pairing.h pbcwrapper/Zr.h pbcwrapper/PBCExceptions.h
pbcwrapper/GT.o: pbcwrapper/GT.h pbcwrapper/G.h pbcwrapper/Pairing.h pbcwrapper/Zr.h pbcwrapper/PBCExceptions.h
pbcwrapper/Pairing.o: pbcwrapper/Pairing.h pbcwrapper/G1.h pbcwrapper/G.h pbcwrapper/Zr.h pbcwrapper/G2.h pbcwrapper/GT.h pbcwrapper/PBCExceptions.h
pbcwrapper/PPPairing.o: pbcwrapper/PPPairing.h pbcwrapper/Pairing.h pbcwrapper/G1.h pbcwrapper/G.h pbcwrapper/Zr.h pbcwrapper/G2.h pbcwrapper/GT.h pbcwrapper/PBCExceptions.h
# Testing.o: pbcwrapper/PBC.h pbcwrapper/G1.h pbcwrapper/G.h pbcwrapper/Pairing.h pbcwrapper/Zr.h pbcwrapper/G2.h pbcwrapper/GT.h pbcwrapper/PBCExceptions.h
# Testing.o: pbcwrapper/PPPairing.h
pbcwrapper/Zr.o: pbcwrapper/Zr.h pbcwrapper/Pairing.h pbcwrapper/PBCExceptions.h
Util/Utils.o: Util/Utils.h
OXT/OXTProtocol.o: OXT/OXTProtocol.h OXT/OXTEDB.h OXT/OXTTSetTuple.h common/data_type.h pbcwrapper/PBC.h Util/Utils.h
OXT/OXTmain.o: OXT/OXTProtocol.h