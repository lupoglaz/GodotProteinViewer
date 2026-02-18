#ifndef GDEXTENSION_CPDB_VIS_H_
#define GDEXTENSION_CPDB_VIS_H_

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/multi_mesh.hpp>
#include <godot_cpp/classes/multi_mesh_instance3d.hpp>
#include <godot_cpp/classes/immediate_mesh.hpp>



#include <PDB/cPDBData.h>

namespace godot{
    class cPDBMeshSimple : public Node3D{
        GDCLASS(cPDBMeshSimple, Node3D)

        // Mesh *line = NULL;
        MultiMeshInstance3D *lines = NULL;
        MultiMeshInstance3D *atoms = NULL;

        public:
            cPDBMeshSimple();
            ~cPDBMeshSimple();
            Ref<cPDBData> pdb_data;
            void set_data(Ref<cPDBData> _data);
            Ref<cPDBData> get_data(){return pdb_data;}

        protected:
            static void _bind_methods();
    };
}
#endif //GDEXTENSION_CPDB_VIS_H_