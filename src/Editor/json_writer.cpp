#include "editor.h"
#include "json_writer.h"
#include "json.hpp"

void JsonWriter::SaveLevelToFile(const char* file_path, Editor* editor)
{
    nlohmann::json json;

    json["TileSize"] = editor->GetTileSize();
    json["Rows"] = editor->GetRowTileCount();
    json["Columns"] = editor->GetColumnTileCount();
    json["CollisionMap"] = editor->GetCollisionMap();

    std::string json_string = to_string(json);
}
