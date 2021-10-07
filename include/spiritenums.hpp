#ifndef SPIRIT_ENUMS_HPP_INCLUDED
#define SPIRIT_ENUMS_HPP_INCLUDED

struct SpiritEnums {
   struct ActiveWindowTracker {	
      struct Error {
         enum : short {
	      NoError = 0,
	      UnSupportedPlatform
         };
      };
   };

   struct Spirit {
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
	      Animating,
	      Stopped,
	      Canceled	      
	  }; 
       };
   };
};

#endif // SPIRIT_ENUMS_HPP_INCLUDED
