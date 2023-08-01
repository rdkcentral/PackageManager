# PackageManager #

The PackageManager service allows installation of packages on the system from a remote repository.
The PackageManager service is a Thunder plugin that implements the IPackageManager interface and can have different implementations for this interface, the implementation to be used is selected at build time. The ExternalService implementation is one of them.

## ExternalService implementation ##

The External Service implementation uses Thunder COMRPC (a (domain)socket based binary protocol) to forward the calls it receives on the IPackageManager interface (so that could be both COMRPC and JSON RPC) to a PackageManager implementation that has registered itself with the ExternalService implementation beforehand. 
So the ExternalService does not implement the PackageManager functionality itself but acts as a broker to an actual implemenation available elsewhere on the system.
The actual implementation must implement the IPackageManager interface as well and can use COMRPC to register itself with the ExternalService in the PackageManager plugin.

## ExampleClient ##

The ExampleClient folder contains a working example on how to implement the Client side for the actual PackageManager implementation and how it should register itself with the ExternalService implementation.
