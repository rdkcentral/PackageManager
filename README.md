# PackageManager #

The Package Manager plugin allows installation of packages on the system from a remote repository.

<br>

### Table of Contents ###

[Contributing to RDKServices](#contributing-to-packagemanager)<br>
[Comcast CI/CD](#comcast-cicd)<br>
[Documentation](#documentation)<br>
[Coding Guidelines](#coding-guidelines)<br>
[Versioning](#versioning)<br>
[Code Review Checklist](#code-review-checklist)<br>
[Questions?](#questions)<br>

## Contributing to PackageManager ##

### **License Requirements** ###
1. Before RDK accepts your code into the project you must sign the RDK [Contributor License Agreement (CLA)](https://developer.rdkcentral.com/source/contribute/contribute/before_you_contribute/).

2. Each new file should include the latest [RDKM license header](https://developer.rdkcentral.com/source/source-code/source-code/coding_guideline/).

3. License for this project is included in the root directory and there shouldn't be any additional license file in any of the subfolders.
<br><br>

### **How to contribute?** ###
1. [Fork](https://docs.github.com/en/github/getting-started-with-github/quickstart/fork-a-repo) the repository, commit your changes, build and test it in at least one approved test platform/device.

2. To test it in a RDKV device, update SRC_URI and SRCREV in the [rdkservices_git.bb](https://gerrit.teamccp.com/plugins/gitiles/rdk/yocto_oe/layers/meta-rdk-video/+/2103_sprint/recipes-extended/rdkservices/rdkservices_git.bb) recipe to point to your fork.

3. Submit your changes as a [pull request](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request-from-a-fork) to the latest sprint branch.

4. If more than one developer has to work on a particular feature, request for a dev branch to be created.
<br><br>

### **Pull request Checklist** ###
1. When a pull request is submitted, blackduck, copyright and cla checks will automatically be triggered. Ensure these checks have passed (turned into green).

2. At least one reviewer needs to **review and approve** the pull request.

3. For tracking and release management purposes, each pull request and all the commits in the pull request shall include **RDK ticket number(s) or Github issue number(s)** and “reason for the change”.

4. Any pull request from Comcast developers should include a link to successful gerrit verification (in the comment section).

5. To verify your changeset in gerrit, submit a test gerrit change to [rdkservices_git.bb](https://gerrit.teamccp.com/plugins/gitiles/rdk/yocto_oe/layers/meta-rdk-video/+/2103_sprint/recipes-extended/rdkservices/rdkservices_git.bb) with the SRC_URI and SRCREV pointing to your fork.

6. If the changes to PackageManager require any Thunder framework changes, the contributor has to plan for a limited regression testing (with the Thunder and PackageManager changes) before submitting the pull request.
<br><br>

## Comcast CI/CD ##

### **Upstream Vs Patch** ###
1. Patches will increase the chances of build failures when the git hash is moved to a newer version.

2. We encourage everyone to upstream all the changes to GitHub instead of using patches

3. On a need basis, a developer can request the maintainers for an approval to use a patch in RDKV build (as a stop-gap measure). An unapproved patch will be rejected.
<br><br>

## Documentation ##

RDK services are described using [JSON Schema](https://json-schema.org/). JSON Schema provides a standard approach for describing APIs and ensures consistency across all APIs. There are two schemas that are used to describe a service:

* [plugin.schema.json](https://github.com/rdkcentral/rdkservices/blob/main/Tools/json_generator/schemas/plugin.schema.json): A schema for defining a service.
* [interface.schema.json](https://github.com/rdkcentral/rdkservices/blob/main/Tools/json_generator/schemas/interface.schema.json): A schema for defining the properties, methods, and events of a service.

Each RDK service has an instance of these schemas in the root of the service folder. For example, `MyServicePlugin.json` and `MyService.json`. These files are used to generate API documentation as Markdown. Each service has a Markdown file that is written to the `docs/api` folder. The following demonstrates the folder structure:

```shell
/rdkservices
    /MyService
        /MyService.json
        /MyServicePlugin.json
    /docs/api
        /MyServicePlugin.md
```

Markdown files are generated from the JSON definitions using the json_generator tool (`Tools/json_generator/generator_json.py`).

The generator tool requires:

* Python 3.5 or higher
* The jsonref library

Verify your Python version:

```shell
python --version
```

Install jsonref if it is not already installed:

```shell
pip install jsonref
```

### Generating Markdown for a Single Service ###

To generate markdown for a single service:

1. Change directories to `Tools/json_generator`.
2. Run `generator_json.py` and provide the location of the service JSON plugin file using the `-d` argument and the output directory using the `-o` argument. You must also include the `--no-interfaces-section` argument; otherwise, an interface section is added to the markdown that links back to the ThunderInterfaces project. Make certain that you are pointing to the plugin definition and not the interface definition. Here is an example of using the tool:

    ```shell
    python ./generator_json.py -d ../../MyService/MyServicePlugin.json  -o ../docs/api --no-interfaces-section --verbose $files
    ```

    The `MyServicePlugin.md` file is written to the `../docs/api` folder. This is the standard directory where all the service API markdown files are written.

## Coding Guidelines ##

1. Be Consistent

    * The point of having style guidelines is to have a common vocabulary of coding so people can concentrate on what you’re saying rather than on how you’re saying it.

    * If the code you add to a file looks drastically different from the existing code around it, it throws readers out of their rhythm. Avoid this.

    * If you’re editing code, take a few minutes to determine the coding style of the component and apply the same style.

    * To maintain uniformity in all text-editors, set TAB size to 2 or 4 spaces and replace TAB by SPACES

    * If they use spaces around all their arithmetic operators, you should too.
    
    * If the comments have little boxes of hash marks around them, make your comments have little boxes of hash marks around them too.

    * Minimise the use of exceptions and handle exceptions locally if possible
    
2. RDK services are implemented as Thunder Plugins and must adhere to the [PluginHost::IPlugin](https://github.com/rdkcentral/Thunder/blob/master/Source/plugins/IPlugin.h) interface. If a RDK service handles WEB requests it implements [PluginHost::IWeb](https://github.com/rdkcentral/Thunder/blob/master/Source/plugins/IPlugin.h). If it activates/deactivates and handles JSON-RPC it implements [PluginHost::IDispatcher](https://github.com/rdkcentral/Thunder/blob/master/Source/plugins/JSONRPC.h) (or derives from PluginHost::JSONRPC). If it implements custom interfaces it adds them to [ThunderInterfaces](https://github.com/rdkcentral/ThunderInterfaces) for RPC. If it exposes interfaces to other processes via RPC it develops an [RPC::Communicator](https://github.com/rdkcentral/Thunder/blob/master/Source/com/Communicator.h). A service specifies its interfaces like this:

```shell
    BEGIN_INTERFACE_MAP(MyPlugin)
    INTERFACE_ENTRY(PluginHost::IPlugin)
    INTERFACE_ENTRY(PluginHost::IDispatcher)
    END_INTERFACE_MAP
```

3. All RDK Services must have a callsign with a prefix of `org.rdk`. RDK Service name must be CamelCase and start with a capital letter.

4. All method, parameter and event names must be camelCase and start with a lowercase letter.

5. MODULE_NAME

    * Thunder provides a trace and warning reporting feature. To accurately identify the source of a warning, Thunder needs to know the human readable name of the package (executable or library). This package name is defined by the MODULE_NAME and declared by the  MODULE_NAME_DECLARATION()

    * Any package that includes a Thunder component requires such a definition and declaration. If the definition is missing, a compiler error will be reported (error missing MODULE_NAME) and if the declaration is missing, a linker error will be reported (missing or duplicate symbol)

    * Module.h defines the mandatory MODULE_NAME. It also includes framework headers (<plugins/plugins.h>)
        ```shell
        #ifndef MODULE_NAME
        #define MODULE_NAME Plugin_IOController
        #endif
        ```
    * Module.cpp defines the mandatory MODULE_NAME_DECLARATION
        ```shell
        #include "Module.h"
        MODULE_NAME_DECLARATION(BUILD_REFERENCE)
        ```

6. A service is registered in a translation unit via a mandatory SERVICE_REGISTRATION(MyService, API_VERSION_NUMBER_MAJOR, API_VERSION_NUMBER_MINOR, API_VERSION_NUMBER_PATCH)

7. MyPlugin.config can specify: autostart, startuporder, custom properties (passed to the service during activation via PluginHost::IShell::ConfigLine()). During the project configuration, write_config(MyPlugin) in CMakeLists.txt uses MyPlugin.config to generate and install a corresponding json file. Refer to [PersistentStore.config](https://github.com/rdkcentral/rdkservices/blob/main//PersistentStore/PersistentStore.config)

8. MyPlugin.json documents JSON-RPC interface. It's added to both plugin folder and ThunderInterfaces. The latter generates classes for parameters or enums that can be included like #include <interfaces/json/JsonData_MyPlugin.h>. In the plugin code, JSON-RPC methods and properties are registered like this:

```shell
    Register<void /*input params*/, void /*output params*/>(_T("sync"), &MyPlugin::endpoint_sync, this);
    Property<LocationData>(_T("location"), &MyPlugin::get_location /*getter*/, nullptr /*setter*/, this);
```

9. Initialization and Cleanup

    * Keep Plugin Constructors & Destructors lean. Most initialization should be done within Initialize() which gets called when the plugin is activated. More on that below. This will ensure that WPEFramework boots up faster since most of the plugins are not auto-started or activated on bootup. Similarly most of the plugin cleanup should happen within Deinitialize() instead of the destructor.

    * Prefer to do Plugin Initialization within IPlugin [Initialize()](https://github.com/rdkcentral/Thunder/blob/master/Source/plugins/IPlugin.h#L71). If there is any error in initialization return non-empty string with useful error information. This will ensure that plugin doesn't get activated and also return this error information to the caller. Ensure that any Initialization done within Initialize() gets cleaned up within IPlugin [Deinitialize()](https://github.com/rdkcentral/Thunder/blob/master/Source/plugins/IPlugin.h#L80) which gets called when the plugin is deactivated.
    
10.  Inter-plugin communication - There might be use cases where one RDK Service or plugin needs to call APIs in another RDK Service. Don't use JSON-RPC for such communication since it's an overhead and not preferred for inter-plugin communication. JSON-RPC must be used only by applications. Instead use COM RPC through the IShell Interface API [QueryInterfaceByCallsign()](https://github.com/rdkcentral/Thunder/blob/R2/Source/plugins/IShell.h#L210) exposed for each Plugin. Here is an [example](https://github.com/rdkcentral/rdkservices/blob/main/Messenger/MessengerSecurity.cpp#L35). 
    <br><br>

## Versioning ##

* Versioning

    * Given a version number MAJOR.MINOR.PATCH, increment the:
        * MAJOR version when you make incompatible API changes that break backwards compatibility. This could be removing existing APIs, changes to API Signature or major changes to API behavior that breaks API contract, 
        * MINOR version when you add backward compatible new features like adding new APIs, adding new parameters to existing APIs,
        * PATCH version when you make backwards compatible bug fixes.

    * RDK Service version in MAJOR.MINOR.PATCH format is updated using SERVICE_REGISTRATION macro.

    ```
    SERVICE_REGISTRATION(DisplaySettings, API_VERSION_NUMBER_MAJOR, API_VERSION_NUMBER_MINOR, API_VERSION_NUMBER_PATCH);
    ```

    * There is also a Plugin::Metadata structure maintained for each RDK Service that keeps the versioning information. This is returned in call to Controller.1.status.

    ```
    static Plugin::Metadata<Plugin::DisplaySettings> metadata(
            // Version (Major, Minor, Patch)
            API_VERSION_NUMBER_MAJOR, API_VERSION_NUMBER_MINOR, API_VERSION_NUMBER_PATCH,
            // Preconditions
            {},
            // Terminations
            {},
            // Controls
            {}
        );
    }
    ```

    * Changes in version should be updated when commits are added to the main or release branches. There should be a version change per JIRA Ticket. This is not enforced on sprint branches since there could be multiple changes for the same JIRA ticket during development.  

* Changelog

    * Each RDK Service has a CHANGELOG file that contains all changes done so far. When version is updated, add a entry in the CHANGELOG.md at the top with user friendly information on what was changed with the new version. Please don't mention JIRA tickets in CHANGELOG. Refer to [Changelog](https://github.com/olivierlacan/keep-a-changelog/blob/main/CHANGELOG.md) as an example and [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) for more details.

    * Please Add entry in the CHANGELOG for each version change and indicate the type of change with these labels:
        * **Added** for new features.
        * **Changed** for changes in existing functionality.
        * **Deprecated** for soon-to-be removed features.
        * **Removed** for now removed features.
        * **Fixed** for any bug fixes.
        * **Security** in case of vulnerabilities.

    * Changes in CHANGELOG should be updated when commits are added to the main or release branches. There should be one CHANGELOG entry per JIRA Ticket. This is not enforced on sprint branches since there could be multiple changes for the same JIRA ticket during development. 

* Deprecation
    * Breaking changes to the API that requires a major version update should first go through Deprecation by doing a minor version update. We recommend atleast 2 RDK releases with the deprecated API/s and minor version update to give time for clients and apps to make changes to remove the deprecated API. Following needs to be done for deprecation.
        * The API/s getting deprecated should be marked with a ["deprecated"](https://github.com/rdkcentral/rdkservices/blob/main/SystemServices/System.json#L287) label in the json schema. This will ensure that it's updated in the API [documentation.](https://rdkcentral.github.io/rdkservices/#/api/SystemPlugin?id=cachecontains)
        * Add a changelog entry with minor version update and include **Deprecated** label to call out the API/s getting deprecated. 
        * If this API/s is getting replaced by a newer API then it can come in the same minor version update with changelog entry with **Added** label.
    <br><br>

## Code Review Checklist ##

This checklist is primarily intended for maintainers or reviewers. Please check for the following before approving Pull Requests.

* [Coding Guidelines](#coding-guidelines) are followed. 
* API Changes are [documented](#documentation) and [versioned](#versioning).
* JIRA Ticket Number is mentioned in the title or commit message for tracking.
* Any Dependent HAL changes are to be merged first before merging changes to RDK Service to avoid build issues.
* For a New RDK Service, ensure [autostart](https://github.com/rdkcentral/rdkservices/blob/main/AVInput/AVInput.config#L1) flag is set to false. The general recommendation is for RDK Services to **not** be autostarted or activated unless it's required on bootup. Resident Apps can activate the required RDK Services on demand.
* Approve Pull Requests to [main](https://github.com/rdkcentral/RDKServices/tree/main) branch or release branches (release/*) only after you get Release Management approval for those specific branches.


## Questions? ##

If you have any questions or concerns reach out to [Anand Kandasamy](mailto:anand_kandasamy@comcast.com)

For a plugin specific question, maintainers might refer you to the plugin owner(s).
<br><br>