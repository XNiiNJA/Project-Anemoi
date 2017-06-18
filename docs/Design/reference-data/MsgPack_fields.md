All of these variables are put into a MsgPack map, sent to the other machine, converted to uint64_t, then converted to whatever type they actually are. MsgPack is weird.

### Flight Dynamics

Vector X direction - "v_x"

Vector Y direction - "v_y"

Vector Z direction - "v_z"

Vector magnitude - "v_mag"

Vector type enum - "t_vect"

Heading - "f_head"

### Connection management

connect to peer ip-address - "p_c2addr"

connect to peer port       - "p_c2port" //For port punching

remove peer ip-address - "p_raddr" //Will remove corresponding port

### Other cool stuff.



