# ======================================================================================
#
#  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
#  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
#  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
#  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
#  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
#  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
#
#  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

target_sources (
	limes_core
	PRIVATE RealtimeTypeTraits.h
			RealtimeSafeObject.h
			ThreadSafeObject.h
			RealtimeSafeObject/ObjectBase.h
			RealtimeSafeObject/AtomicObjectHolder.h
			RealtimeSafeObject/NonrealtimeMutatableObject.h
			RealtimeSafeObject/RealtimeMutatableObject.h
			RealtimeSafeObject/RealtimeSafeObject_impl.h)

target_include_directories (limes_core PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
