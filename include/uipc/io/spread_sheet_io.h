#pragma once
#include <uipc/geometry/simplicial_complex.h>
namespace uipc::geometry
{
/**
 * @brief A class for writing geometry spreadsheet.
 * 
 * @code
 *  SimplicialComplexIO io;
 *  auto mesh = io.read("cube.msh");
 *  SpreadSheetIO sio;
 *  // dump to csv
 *  sio.write_csv("spreadsheet", mesh);
 *  // dump to json
 *  sio.write_json("spreadsheet", mesh);
 * @endcode
 */
class UIPC_IO_API SpreadSheetIO
{
  public:
    SpreadSheetIO(std::string_view output_folder = "./");
    void write_json(std::string_view geo_name, const SimplicialComplex& simplicial_complex) const;
    void write_json(const SimplicialComplex& simplicial_complex) const;
    void write_csv(std::string_view geo_name, const SimplicialComplex& simplicial_complex) const;
    void write_csv(const SimplicialComplex& simplicial_complex) const;

  private:
    string m_output_folder;
};
}  // namespace uipc::geometry
