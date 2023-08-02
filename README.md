# PackageManager #

The code manages packages by providing functionality to install and remove packages, as well as retrieve metadata for installed packages.
The PackageManager repository contains multiple implementations of the same [interface](https://github.com/rdkcentral/PackageManager/blob/main/doc/PackageManagerPlugin.md) which can be [selected](https://github.com/rdkcentral/PackageManager/blob/main/CMakeLists.txt#L54) compile time.

### Current implementations 
 - [Test implementation](https://github.com/rdkcentral/PackageManager/tree/main/TestImplementation)
 - [External services implementation](https://github.com/rdkcentral/PackageManager/tree/main/ExternalServiceImplementation)

### Thunder plugin

The PackageManager is a [Thunder](https://github.com/rdkcentral/Thunder) plugin.
Please refer to the [Thunder documentation](https://rdkcentral.github.io/Thunder/) for more information on Thunder.


