//
// Copyright (c) 2023 Paper Cranes Ltd.
// All rights reserved.
//
#include <godot_cpp/godot.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <cPDBData.h>
#include <cPDBVis.h>

static godot::Ref<godot::cPDBFileLoader> pdb_loader;

void register_gameplay_types(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    godot::ClassDB::register_class<godot::cPDBData>();
    godot::ClassDB::register_class<godot::cPDBMeshSimple>();
    
    godot::ClassDB::register_class<godot::cPDBFileLoader>();
	pdb_loader.instantiate();
	godot::ResourceLoader::get_singleton()->add_resource_format_loader(pdb_loader);
}

void unregister_gameplay_types(godot::ModuleInitializationLevel p_level) {
}

extern "C" {

GDExtensionBool GDE_EXPORT gameplay_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(register_gameplay_types);
	init_obj.register_terminator(unregister_gameplay_types);
	init_obj.set_minimum_library_initialization_level(godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}

}