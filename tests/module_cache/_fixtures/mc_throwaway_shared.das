options gen2
options indenting = 4

module mc_throwaway_shared shared public

//! the tuple's first element is named for the module
//! mc_throwaway_second requires after this one, so that require's
//! name is first read in this module's record
def public take(t : tuple<mc_throwaway_helper : int; z : float>) : int => t.mc_throwaway_helper
