#ifndef GDEXTENSION_CDSSP_H_
#define GDEXTENSION_CDSSP_H_

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/immediate_mesh.hpp>



#include <PDB/cPDBData.h>

namespace godot{
    enum StructureType {
        LOOP,
        ALPHA_HELIX,
        BETA_BRIDGE,
        STRAND,
        HELIX_3,
        HELIX_5,
        HELIX_PPII,
        TURN,
        BEND,
        GAP
    };

    struct StructElem {
        uint res_start, res_end;
        StructureType type;
    };

    const float BOND_LENGTH_NH = 1.01,
                BOND_LENGTH_OH = 0.96,
                ANGLE_INTERNAL_NH = 120.0,
                ANGLE_TERMINAL_NH2 = 109.5,
                ANGLE_TERMINAL_OH = 109.5;


    class cDSSP : public RefCounted{
        GDCLASS(cDSSP, RefCounted)
        MemArena<PDBAtom, 8> hydrogen_atoms;

        public:
            cDSSP(){};
            ~cDSSP(){};
            void add_backbone_hydrogens(Ref<cPDBData> data);
            

        protected:
            static void _bind_methods();
    };
}
#endif //GDEXTENSION_CPDB_VIS_H_