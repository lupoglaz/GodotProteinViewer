#include <cPDBData.h>
#include <godot_cpp/core/class_db.hpp>

namespace godot{
    void _my_convert_string(const String &src, char *dst, int num_char){
        for(int i=0; i<num_char; i++){
            if(src.length() > i)
                dst[i] = (char)src[i];
            else
                dst[i] = '\0';
        }
    }
    void cPDBData::load(const String &path){
        Ref<FileAccess> data = FileAccess::open(path, FileAccess::READ);
        int _record_num = 0;
        for(String line = data->get_line(); !data->eof_reached(); line = data->get_line()){
            if(line.length() >= 6){
                String record_name = line.substr(0, 6);
                if(record_name.casecmp_to("MODEL ") == 0){
                    String record_num = line.substr(6, -1).strip_edges();
                    if(!record_num.is_valid_int()){
                        UtilityFunctions::print(line);
                        UtilityFunctions::print("Error model num is not valid: ", record_num);
                        return;
                    }
                    _record_num = record_num.to_int();
                }else if((record_name.casecmp_to("HETATM")==0) || (record_name.casecmp_to("ATOM  ")==0)){
                    String atom_serial_num = line.substr(6, 5).strip_edges();
                    if(!atom_serial_num.is_valid_int()){
                        UtilityFunctions::print(line);
                        UtilityFunctions::print("Error atom serial num is not valid: ", atom_serial_num);
                        return;
                    }
                    
                    String atom_name = line.substr(13, 4).strip_edges();
                    String res_name = line.substr(17, 3).strip_edges();
                    String chain_name = line.substr(21, 1).strip_edges();
                    String res_serial_num = line.substr(22, 4).strip_edges();
                    if(!res_serial_num.is_valid_int()){
                        UtilityFunctions::print(line);
                        UtilityFunctions::print("Error residue serial num is not valid: ", res_serial_num);
                        return;
                    }
                    
                    String atom_pos_x = line.substr(30, 8).strip_edges();
                    String atom_pos_y = line.substr(38, 8).strip_edges();
                    String atom_pos_z = line.substr(46, 8).strip_edges();
                    if(!atom_pos_x.is_valid_float() || !atom_pos_y.is_valid_float() || !atom_pos_z.is_valid_float()){
                        UtilityFunctions::print(line);
                        UtilityFunctions::print("Error atom position is not valid: (", atom_pos_x, "; ", atom_pos_y, "; ", atom_pos_z, ")");
                        return;
                    }
                    String atom_symbol = line.substr(76, -1).strip_edges();
                    
                    PDBAtom atom = {
                        _record_num,//model_num
                        record_name.casecmp_to("HETATM")==0,//is_heterogen
                        (int)atom_serial_num.to_int(),//serial
                        {'\0','\0','\0','\0','\0'},//name[5]
                        (int)res_serial_num.to_int(),//res_seq
                        {'\0','\0','\0','\0'},//res_name
                        {'\0'},//chain_id
                        {' '},//i_code
                        {float(atom_pos_x.to_float()), float(atom_pos_y.to_float()), float(atom_pos_z.to_float())},//position[3]
                        {'\0','\0','\0'}//symbol[3]
                    };
                    _my_convert_string(atom_name, atom.name, 5);
                    _my_convert_string(res_name, atom.res_name, 4);
                    _my_convert_string(atom_symbol, atom.symbol, 3);
                    atom.chain_id = chain_name[0];
                    atoms.add_element(atom);

                }else if(record_name.casecmp_to("CONECT")==0){
                    String atom_serial_num = line.substr(6, 5).strip_edges();
                    if(!atom_serial_num.is_valid_int()){
                        UtilityFunctions::print(line);
                        UtilityFunctions::print("Error atom serial num is not valid: ", atom_serial_num);
                        return;
                    }
                    int _atom_serial_num = atom_serial_num.to_int();
                    int _atom_match_num[4];
                    for(int i=0; i<3; i++){
                        String atom_match_num = line.substr(11 + 5*i, 5).strip_edges();
                        if(!atom_match_num.is_valid_int())
                            break;
                        _atom_match_num[i] = atom_match_num.to_int();
                    }
                }else if(record_name.casecmp_to("HELIX")==0){
                    
                }else if(record_name.casecmp_to("SHEET ")==0){

                }else if(record_name.casecmp_to("ENDMDL")==0){

                }

            }
        }
    }

    void cPDBData::_bind_methods() {
        // ClassDB::bind_method(D_METHOD("set_max_num_points", "max_num_points"), &cAssetPlacerData::set_max_num_points);
        // ClassDB::bind_method(D_METHOD("get_max_num_points"), &cAssetPlacerData::get_max_num_points);
        // ClassDB::add_property("cAssetPlacerData", PropertyInfo(Variant::FLOAT, "max_num_points"), "set_max_num_points", "get_max_num_points");
    }
}