#ifndef SPIRIT_ENUMS_HPP_INCLUDED
#define SPIRIT_ENUMS_HPP_INCLUDED

#ifndef SPIRIT_VERSION
#define SPIRIT_VERSION "0.0.0"
#endif // SPIRIT_VERSION

#ifndef SPIRIT_COMMIT
#define SPIRIT_COMMIT "dev"
#endif // SPIRIT_COMMIT

#ifndef SPIRIT_BUILD
#define SPIRIT_BUILD "none"
#endif // SPIRIT_BUILD

struct SpiritEnums {
    struct ActiveWindowTracker {
        struct Error {
            enum : short {
                NoError = 0,
                UnSupportedPlatform
            };
        };
    };

    struct SpiritFile {
        struct Error {
            enum : short {
                NoError = 0,
                NoSpiritFile,
                InvalidSpiritFile,
                InvalidSpiritEdition,
                CannotFindMetaFile,
                CannotParseMetaFile,
                SpiritNotCanceled,
                CannotGetDefaultAction,
                CannotGetAction
            };
        };

        struct Status {
            enum : short {
                Idle = 0,
                Errored,
                Loading,
                Parsing,
                Loaded,
                Canceled
            };
        };

    };

    struct Spirit {
        struct Position {
            enum : short {
                TopRight = 0,
                TopLeft,
                BottomRight,
                BottomLeft
            };
        };

        struct Error {
            enum : short {
                NoError = 0
            };
        };

        struct Status {
            enum : short {
                Idle = 0,
                Errored
            };
        };
    };
};

#endif // SPIRIT_ENUMS_HPP_INCLUDED
