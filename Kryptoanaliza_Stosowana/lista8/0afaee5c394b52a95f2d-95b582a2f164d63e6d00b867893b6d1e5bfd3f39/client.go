package main

import (
	"crypto/tls"
	"crypto/x509"
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
)

var (
	flServerCertFile = flag.String("ca", "", "")
	flClientCertFile = flag.String("cert", "", "")
	flClientKeyFile  = flag.String("key", "", "")
	flAddr           = flag.String("addr", os.Getenv("R_PORT_443_TCP_ADDR"), "")
	flPort           = flag.String("port", os.Getenv("R_PORT_443_TCP_PORT"), "")
)

func main() {
	flag.Parse()
	var (
		certs          []tls.Certificate
		serverCertData []byte
		certData       []byte
		keyData        []byte
		err            error
	)

	if len(*flServerCertFile) == 0 || len(*flClientCertFile) == 0 || len(*flClientKeyFile) == 0 {
		log.Fatal("provide -ca, -cert, and -key")
	}
	if serverCertData, err = ReadFile(*flServerCertFile); err != nil {
		log.Fatalf("%q: %s", *flServerCertFile, err)
	}
	if certData, err = ReadFile(*flClientCertFile); err != nil {
		log.Fatalf("%q: %s", *flClientCertFile, err)
	}
	if keyData, err = ReadFile(*flClientKeyFile); err != nil {
		log.Fatalf("%q: %s", *flClientKeyFile, err)
	}

	if len(*flAddr) == 0 || len(*flPort) == 0 {
		log.Fatal("provide -addr and -port of the server")
	}

	pool := x509.NewCertPool()
	pool.AppendCertsFromPEM([]byte(serverCertData))
	cert, err := tls.X509KeyPair([]byte(certData), []byte(keyData))
	if err != nil {
		fmt.Println(err)
		return
	}
	certs = append(certs, cert)

	tlsConfig := tls.Config{
		RootCAs: pool,
		// Avoid fallback to SSL protocols < TLS1.0
		MinVersion:   tls.VersionTLS10,
		MaxVersion:   tls.VersionTLS10,
		Certificates: certs,
	}

	httpTransport := &http.Transport{
		//DisableKeepAlives: true,
		Proxy:           http.ProxyFromEnvironment,
		TLSClientConfig: &tlsConfig,
	}

	client := &http.Client{
		Transport: httpTransport,
	}

	// SSLVerifyClient optional
	res, err := client.Get(fmt.Sprintf("https://%s:%s/", *flAddr, *flPort))
	if err != nil {
		log.Fatal("FATAL: ", err)
	}
	defer res.Body.Close()
	if buf, err := ioutil.ReadAll(res.Body); err != nil {
		log.Fatal("ERROR: ", err)
	} else {
		log.Printf("GOT: %s", buf)
	}
	fmt.Println(res)

	// SSLVerifyClient require
	res, err = client.Get(fmt.Sprintf("https://%s:%s/secret", *flAddr, *flPort))
	if err != nil {
		log.Fatal("FATAL: ", err)
	}
	defer res.Body.Close()
	if buf, err := ioutil.ReadAll(res.Body); err != nil {
		log.Println("ERROR: ", err)
	} else {
		log.Printf("GOT: %s", buf)
	}
	fmt.Println(res)
}

func ReadFile(path string) ([]byte, error) {
	fh, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer fh.Close()
	return ioutil.ReadAll(fh)
}
