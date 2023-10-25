# PackageManager #

The PackageManager service allows installation of packages on the system from a remote repository.
The PackageManager service is a Thunder plugin that implements the IPackageManager interface and can have different implementations for this interface, the implementation to be used is selected at build time. The ExternalService implementation is one of them.

## Test implementation ##

The test implementation is created purely for automated testing of the IPackageManager interface. The plugin does not install / remove packages or do anything other then give static replies to the API calls.
