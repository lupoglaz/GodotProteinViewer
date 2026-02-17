#ifndef GDEXTENSION_CPDB_DATA_H_
#define GDEXTENSION_CPDB_DATA_H_

#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/classes/image.hpp> //Resource class decl

#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/classes/resource_format_saver.hpp>
#include <godot_cpp/classes/file_access.hpp>

#include <Common/DataTypes.h>

namespace godot{

struct PDBAtom {
	// -1 indicates heterogen_model, otherwise index of model
	int model_num;
	bool is_heterogen;
	int serial;
	char name[5] = "";
	int res_seq;
	char res_name[4];
	char chain_id;
	char i_code;
	float position[3];    // Orthogonal coordinates in Angstroms
	char symbol[3] = "";
};

class cPDBData : public Resource {
	GDCLASS(cPDBData, Resource)
	unsigned long num_atoms = 0;
	MemArena<PDBAtom, 32> atoms;
	public:
		cPDBData(){};
		~cPDBData(){};
		void load(const String &path);
	protected:
		static void _bind_methods();
};

class cPDBFileLoader : public ResourceFormatLoader {
    GDCLASS(cPDBFileLoader, ResourceFormatLoader)
	public:
		Variant _load(const String &path, const String &original_path, bool use_sub_threads, int32_t cache_mode) const override{
			Ref<cPDBData> pdb(memnew(cPDBData));
			pdb->load(path);
			return pdb;
		}
		PackedStringArray _get_recognized_extensions() const override{
			PackedStringArray ext;
			ext.append("pdb");
			return ext;
		}
		bool _handles_type(const StringName &type) const override{
			return type.to_lower().contains("cpdbdata");
		}
		String _get_resource_type(const String &p_path) const override{
			String el = p_path.get_extension().to_lower();
			if (el == "pdb") {
				return String("cPDBData");
			}
			return String("");
		}
	protected:
		static void _bind_methods(){}
};

}

#endif //GDEXTENSION_CPDB_DATA_H_