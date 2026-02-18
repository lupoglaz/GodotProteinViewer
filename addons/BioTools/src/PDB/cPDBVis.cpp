#include <cPDBVis.h>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/cylinder_mesh.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/variant/transform3d.hpp>

namespace godot{
	cPDBMeshSimple::cPDBMeshSimple(){
		lines = memnew(MultiMeshInstance3D);
		atoms = memnew(MultiMeshInstance3D);
		// line = memnew(Mesh);
		
	}
	cPDBMeshSimple::~cPDBMeshSimple(){
		// if(lines!=NULL)memfree(lines);
		// if(line!=NULL)memfree(line);
	}
	void cPDBMeshSimple::set_data(Ref<cPDBData> _data){
		if(!_data.is_valid()){
			pdb_data = Ref<cPDBData>(NULL);
			return;
		}
		pdb_data = _data;						
		PDBAtom *prev_atom = NULL;
		PackedVector3Array r0, r1;
		for(unsigned long i=0; i<pdb_data->atoms.get_length(); i++){
			PDBAtom *atom = pdb_data->atoms.get_pointer(i);
			if(strcmp(atom->name, "CA")==0){
				if(prev_atom != NULL){
					if(prev_atom->res_seq + 1 == atom->res_seq && atom->chain_id == prev_atom->chain_id){
						r0.push_back(Vector3(prev_atom->position[0], prev_atom->position[1], prev_atom->position[2]));
						r1.push_back(Vector3(atom->position[0], atom->position[1], atom->position[2]));
						UtilityFunctions::print(atom->res_seq, "-",prev_atom->res_seq, " ", atom->chain_id, "-", prev_atom->chain_id);
					}
				}
				prev_atom = atom;
			}
		}

		Ref<CylinderMesh> line;
		line.instantiate();
		line->set_height(1.0);
		line->set_top_radius(0.3);
		line->set_bottom_radius(0.3);
		line->set_radial_segments(8);

		Ref<MultiMesh> multimesh;
		multimesh.instantiate();
		multimesh->set_transform_format(MultiMesh::TRANSFORM_3D);
		multimesh->set_mesh(line);
		multimesh->set_instance_count(r0.size());
		UtilityFunctions::print("Num entities:", r0.size());
		for(size_t i=0;i<r0.size();i++){
			float scale = (r1[i] - r0[i]).length();
			Transform3D T;
			T.translate_local(Vector3(0.0, 0.5, 0.0));
			T.rotate(Vector3(1.0, 0.0, 0.0), -M_PI_2);
			T.scale(Vector3(1.0, 1.0, scale));
			Transform3D Tr;
			Tr.set_look_at(Vector3(0,0,0), r1[i] - r0[i]);
			T = Tr * T;
			multimesh->set_instance_transform(i, T.translated(r0[i]));
		}
		lines->set_multimesh(multimesh);
		this->add_child(lines);

		Ref<SphereMesh> sphere;
		sphere.instantiate();
		sphere->set_radius(0.5);
		sphere->set_radial_segments(8);

		Ref<MultiMesh> multimesh_atoms;
		multimesh_atoms.instantiate();
		multimesh_atoms->set_transform_format(MultiMesh::TRANSFORM_3D);
		multimesh_atoms->set_mesh(sphere);
		multimesh_atoms->set_instance_count(r0.size() + r1.size());
		UtilityFunctions::print("Num entities:", r0.size());
		for(size_t i=0;i<r0.size();i++){
			Vector3 b1(1.0, 0.0, 0.0);
			Vector3 b2(0.0, 1.0, 0.0);
			Vector3 b3(0.0, 0.0, 1.0);
			Transform3D T0 = Transform3D(Basis(b1, b2, b3), r0[i]);
			Transform3D T1 = Transform3D(Basis(b1, b2, b3), r0[i]);
			multimesh_atoms->set_instance_transform(i, T0);
			multimesh_atoms->set_instance_transform(i, T1);
		}
		atoms->set_multimesh(multimesh_atoms);
		this->add_child(atoms);
	}
	void cPDBMeshSimple::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_data", "data"), &cPDBMeshSimple::set_data);
		ClassDB::bind_method(D_METHOD("get_data"), &cPDBMeshSimple::get_data);
		ClassDB::add_property("cPDBMeshSimple", PropertyInfo(Variant::OBJECT, "pdb_data", PROPERTY_HINT_RESOURCE_TYPE), 
			"set_data", "get_data");
	}
}