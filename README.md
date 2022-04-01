# RIL Adapter<a name="EN-US_TOPIC_0000001106353742"></a>

-   [Introduction](#section117mcpsimp)
-   [Directory Structure](#section122mcpsimp)
-   [Constraints](#section126mcpsimp)
-   [Usage Guidelines](#section264mcpsimp)
-   [Repositories Involved](#section279mcpsimp)

## Introduction<a name="section117mcpsimp"></a>

The RIL Adapter module provides functions such as vendor library loading, service interface implementation, and event scheduling and management. The module shields the differences of modems supplied by different vendors to provide a unified interface for the telephony service layer. It communicates with the telephony service layer by registering a Hardware Driver Foundation \(HDF\) service.

**Figure  1**  Architecture of the RIL Adapter module<a name="fig1675210296494"></a>
![](figures/en-us_architecture-of-the-ril-adapter-module.png)

## Directory Structure<a name="section122mcpsimp"></a>

```shell
base/telephony/ril_adapter
├── figures                             # Resource file of readme
├── frameworks
│   ├── BUILD.gn
│   └── src                             # Serialized file
├── interfaces                          # Provide internal interfaces for each business
│   └── innerkits
├── services                            # Service
│   ├── hril                            # Implementation of each business module interface of the hri layer
│   ├── hril_hdf                        # HDF service
│   └── vendor                          # vendor file
│── test                                # Test code
│   ├── BUILD.gn
│   └── unittest                        # Unit test code
└── utils                               # Utilities
```

## Constraints<a name="section126mcpsimp"></a>

-   In terms of software, the RIL Adapter needs to work with the telephony core service \(core\_service\) and an interworking interface needs to be implemented by the modem vendor lib.
-   In terms of hardware, the device must be equipped with a modem capable of independent cellular communication.

## Usage Guidelines<a name="section264mcpsimp"></a>

The RIL Adapter does not provide external APIs and can only be called by the telephony core service.

## Repositories Involved<a name="section279mcpsimp"></a>

[Telephony](https://gitee.com/openharmony/docs/blob/master/en/readme/telephony.md)

**telephony_ril_adapter**

[telephony_core_service](https://gitee.com/openharmony/telephony_core_service/blob/master/README.md)
