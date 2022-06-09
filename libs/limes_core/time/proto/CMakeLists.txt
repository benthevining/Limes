# ======================================================================================
#  __    ____  __  __  ____  ___
# (  )  (_  _)(  \/  )( ___)/ __)
#  )(__  _)(_  )    (  )__) \__ \
# (____)(____)(_/\/\_)(____)(___/
#
#  This file is part of the Limes open source library and is licensed under the terms of the GNU Public License.
#
# ======================================================================================

set (time_proto_files date.proto datetime.proto time.proto)

list (TRANSFORM time_proto_files PREPEND time/proto/)

set (time_proto_files ${time_proto_files} PARENT_SCOPE)
