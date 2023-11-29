
SERVERNAME   := renegotiation-server
CLIENTNAME   := renegotiation-client
DOCKER := docker

all: .build

.build: Dockerfile test-ca.crt server.crt server.key ssl.conf client.crt client.key run.sh client.go
	$(DOCKER) build -t $(SERVERNAME) . && touch $@

run: .build
	$(DOCKER) kill $(SERVERNAME) ; $(DOCKER) rm $(SERVERNAME) ; \
	$(DOCKER) run -it --net=host --name $(SERVERNAME) -P $(SERVERNAME)

run-debug: .build
	$(DOCKER) kill $(SERVERNAME) ; $(DOCKER) rm $(SERVERNAME) ; \
	$(DOCKER) run -it --net=host --name $(SERVERNAME) -P $(SERVERNAME) bash

clean:
	$(DOCKER) kill $(SERVERNAME) ; $(DOCKER) rm $(SERVERNAME) ; \
	rm -f .build *~

