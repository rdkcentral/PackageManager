<!-- Generated automatically, DO NOT EDIT! -->
<a name="head.PackageManager_Plugin"></a>
# PackageManager Plugin

**Version: 1.0**

**Status: :black_circle::black_circle::black_circle:**

PackageManager plugin for Thunder framework.

### Table of Contents

- [Introduction](#head.Introduction)
- [Description](#head.Description)
- [Configuration](#head.Configuration)
- [Interfaces](#head.Interfaces)
- [Methods](#head.Methods)
- [Notifications](#head.Notifications)

<a name="head.Introduction"></a>
# Introduction

<a name="head.Scope"></a>
## Scope

This document describes purpose and functionality of the PackageManager plugin. It includes detailed specification about its configuration, methods provided and notifications sent.

<a name="head.Case_Sensitivity"></a>
## Case Sensitivity

All identifiers of the interfaces described in this document are case-sensitive. Thus, unless stated otherwise, all keywords, entities, properties, relations and actions should be treated as such.

<a name="head.Acronyms,_Abbreviations_and_Terms"></a>
## Acronyms, Abbreviations and Terms

The table below provides and overview of acronyms used in this document and their definitions.

| Acronym | Description |
| :-------- | :-------- |
| <a name="acronym.API">API</a> | Application Programming Interface |
| <a name="acronym.HTTP">HTTP</a> | Hypertext Transfer Protocol |
| <a name="acronym.JSON">JSON</a> | JavaScript Object Notation; a data interchange format |
| <a name="acronym.JSON-RPC">JSON-RPC</a> | A remote procedure call protocol encoded in JSON |

The table below provides and overview of terms and abbreviations used in this document and their definitions.

| Term | Description |
| :-------- | :-------- |
| <a name="term.callsign">callsign</a> | The name given to an instance of a plugin. One plugin can be instantiated multiple times, but each instance the instance name, callsign, must be unique. |

<a name="head.References"></a>
## References

| Ref ID | Description |
| :-------- | :-------- |
| <a name="ref.HTTP">[HTTP](http://www.w3.org/Protocols)</a> | HTTP specification |
| <a name="ref.JSON-RPC">[JSON-RPC](https://www.jsonrpc.org/specification)</a> | JSON-RPC 2.0 specification |
| <a name="ref.JSON">[JSON](http://www.json.org/)</a> | JSON specification |
| <a name="ref.Thunder">[Thunder](https://github.com/WebPlatformForEmbedded/Thunder/blob/master/doc/WPE%20-%20API%20-%20Thunder.docx)</a> | Thunder API Reference |

<a name="head.Description"></a>
# Description

The Package Manager plugin allows installation of packages on the system from a remote repository.

The plugin is designed to be loaded and executed within the Thunder framework. For more information about the framework refer to [[Thunder](#ref.Thunder)].

<a name="head.Configuration"></a>
# Configuration

The table below lists configuration options of the plugin.

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| callsign | string | Plugin instance name (default: *PackageManager*) |
| classname | string | Class name: *PackageManager* |
| locator | string | Library name: *libThunderPackageManager.so* |
| autostart | boolean | Determines if the plugin shall be started automatically along with the framework |

<a name="head.Interfaces"></a>
# Interfaces

This plugin implements the following interfaces:

- IPackageManager ([IPackageManager.h](https://github.com/rdkcentral/ThunderInterfaces/blob/master/interfaces/IPackageManager.h)) (version 1.0.0) (compliant format)

<a name="head.Methods"></a>
# Methods

The following methods are provided by the PackageManager plugin:

PackageManager interface methods:

| Method | Description |
| :-------- | :-------- |
| [install](#method.install) | Download the application bundle |
| [uninstall](#method.uninstall) | Uninstall the application |
| [download](#method.download) | Download arbitrary application's resource file |
| [reset](#method.reset) | Delete persistent data stored locally |
| [getstoragedetails](#method.getstoragedetails) | Information on the storage usage |
| [setauxmetadata](#method.setauxmetadata) | Set an arbitrary metadata |
| [clearauxmetadata](#method.clearauxmetadata) | Clears an arbitrary metadata |
| [getmetadata](#method.getmetadata) | Get application metadata |
| [cancel](#method.cancel) | Cancel asynchronous request |
| [getprogress](#method.getprogress) | Estimated progress of a request |
| [getlist](#method.getlist) | List installed applications |
| [lock](#method.lock) | Lock the application |
| [unlock](#method.unlock) | Unlock application |
| [getlockinfo](#method.getlockinfo) | Get lock info |

<a name="method.install"></a>
## *install [<sup>method</sup>](#head.Methods)*

Download the application bundle.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |
| params.url | string |  |
| params.appname | string |  |
| params.category | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| handle | string |  |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.install",
  "params": {
    "type": "...",
    "id": "...",
    "version": "...",
    "url": "...",
    "appname": "...",
    "category": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "..."
}
```

<a name="method.uninstall"></a>
## *uninstall [<sup>method</sup>](#head.Methods)*

Uninstall the application.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |
| params.uninstalltype | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| handle | string |  |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.uninstall",
  "params": {
    "type": "...",
    "id": "...",
    "version": "...",
    "uninstalltype": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "..."
}
```

<a name="method.download"></a>
## *download [<sup>method</sup>](#head.Methods)*

Download arbitrary application's resource file.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |
| params.reskey | string |  |
| params.url | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| handle | string |  |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.download",
  "params": {
    "type": "...",
    "id": "...",
    "version": "...",
    "reskey": "...",
    "url": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "..."
}
```

<a name="method.reset"></a>
## *reset [<sup>method</sup>](#head.Methods)*

Delete persistent data stored locally.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |
| params.resettype | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | null | Always null |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.reset",
  "params": {
    "type": "...",
    "id": "...",
    "version": "...",
    "resettype": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": null
}
```

<a name="method.getstoragedetails"></a>
## *getstoragedetails [<sup>method</sup>](#head.Methods)*

Information on the storage usage.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| storageinfo | object |  |
| storageinfo.apps | object |  |
| storageinfo.apps.path | string |  |
| storageinfo.apps.quotakb | string |  |
| storageinfo.apps.usedkb | string |  |
| storageinfo.persistent | object |  |
| storageinfo.persistent.path | string |  |
| storageinfo.persistent.quotakb | string |  |
| storageinfo.persistent.usedkb | string |  |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.getstoragedetails",
  "params": {
    "type": "...",
    "id": "...",
    "version": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "apps": {
      "path": "...",
      "quotakb": "...",
      "usedkb": "..."
    },
    "persistent": {
      "path": "...",
      "quotakb": "...",
      "usedkb": "..."
    }
  }
}
```

<a name="method.setauxmetadata"></a>
## *setauxmetadata [<sup>method</sup>](#head.Methods)*

Set an arbitrary metadata.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |
| params.key | string |  |
| params.value | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | null | Always null |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.setauxmetadata",
  "params": {
    "type": "...",
    "id": "...",
    "version": "...",
    "key": "...",
    "value": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": null
}
```

<a name="method.clearauxmetadata"></a>
## *clearauxmetadata [<sup>method</sup>](#head.Methods)*

Clears an arbitrary metadata.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |
| params.key | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | null | Always null |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.clearauxmetadata",
  "params": {
    "type": "...",
    "id": "...",
    "version": "...",
    "key": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": null
}
```

<a name="method.getmetadata"></a>
## *getmetadata [<sup>method</sup>](#head.Methods)*

Get application metadata.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.metadata | object |  |
| result.metadata.appname | string |  |
| result.metadata.type | string |  |
| result.metadata.category | string |  |
| result.metadata.url | string |  |
| result.resources | array |  |
| result.resources[#] | object |  |
| result.resources[#].key | string |  |
| result.resources[#].value | string |  |
| result.auxmetadata | array |  |
| result.auxmetadata[#] | object |  |
| result.auxmetadata[#].key | string |  |
| result.auxmetadata[#].value | string |  |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.getmetadata",
  "params": {
    "type": "...",
    "id": "...",
    "version": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "metadata": {
      "appname": "...",
      "type": "...",
      "category": "...",
      "url": "..."
    },
    "resources": [
      {
        "key": "...",
        "value": "..."
      }
    ],
    "auxmetadata": [
      {
        "key": "...",
        "value": "..."
      }
    ]
  }
}
```

<a name="method.cancel"></a>
## *cancel [<sup>method</sup>](#head.Methods)*

Cancel asynchronous request.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.handle | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | null | Always null |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.cancel",
  "params": {
    "handle": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": null
}
```

<a name="method.getprogress"></a>
## *getprogress [<sup>method</sup>](#head.Methods)*

Estimated progress of a request.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.handle | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| progress | integer |  |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.getprogress",
  "params": {
    "handle": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": 0
}
```

<a name="method.getlist"></a>
## *getlist [<sup>method</sup>](#head.Methods)*

List installed applications.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |
| params.appname | string |  |
| params.category | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| installedids | array |  |
| installedids[#] | object |  |
| installedids[#].id | string |  |
| installedids[#].version | string |  |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.getlist",
  "params": {
    "type": "...",
    "id": "...",
    "version": "...",
    "appname": "...",
    "category": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": [
    {
      "id": "...",
      "version": "..."
    }
  ]
}
```

<a name="method.lock"></a>
## *lock [<sup>method</sup>](#head.Methods)*

Lock the application. Preventing uninstallation.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |
| params.reason | string |  |
| params.owner | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| handle | string |  |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.lock",
  "params": {
    "type": "...",
    "id": "...",
    "version": "...",
    "reason": "...",
    "owner": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": "..."
}
```

<a name="method.unlock"></a>
## *unlock [<sup>method</sup>](#head.Methods)*

Unlock application.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.handle | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | null | Always null |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.unlock",
  "params": {
    "handle": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": null
}
```

<a name="method.getlockinfo"></a>
## *getlockinfo [<sup>method</sup>](#head.Methods)*

Get lock info.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |

### Result

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| result | object |  |
| result.reason | string |  |
| result.owner | string |  |

### Example

#### Request

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "method": "PackageManager.1.getlockinfo",
  "params": {
    "type": "...",
    "id": "...",
    "version": "..."
  }
}
```

#### Response

```json
{
  "jsonrpc": "2.0",
  "id": 42,
  "result": {
    "reason": "...",
    "owner": "..."
  }
}
```

<a name="head.Notifications"></a>
# Notifications

Notifications are autonomous events triggered by the internals of the implementation and broadcasted via JSON-RPC to all registered observers. Refer to [[Thunder](#ref.Thunder)] for information on how to register for a notification.

The following events are provided by the PackageManager plugin:

PackageManager interface events:

| Event | Description |
| :-------- | :-------- |
| [operationstatus](#event.operationstatus) | Completion of asynchronous operation |

<a name="event.operationstatus"></a>
## *operationstatus [<sup>event</sup>](#head.Notifications)*

Completion of asynchronous operation.

### Parameters

| Name | Type | Description |
| :-------- | :-------- | :-------- |
| params | object |  |
| params.handle | string |  |
| params.operation | string |  |
| params.type | string |  |
| params.id | string |  |
| params.version | string |  |
| params.status | string |  |
| params.details | string |  |

### Example

```json
{
  "jsonrpc": "2.0",
  "method": "client.events.1.operationstatus",
  "params": {
    "handle": "...",
    "operation": "...",
    "type": "...",
    "id": "...",
    "version": "...",
    "status": "...",
    "details": "..."
  }
}
```

