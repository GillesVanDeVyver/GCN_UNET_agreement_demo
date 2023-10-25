#include "DistanceNetProcessor.hpp"
#include <FAST/Data/Image.hpp>
#include "FAST/Data/Tensor.hpp"
#include "FAST/Data/Mesh.hpp"
#include <vector>
#include <math.h>

namespace fast {

    void DistanceNetProcessor::init() {
        createInputPort<Tensor>(0);
        createInputPort<Tensor>(1);
        createOutputPort<Image>(0);
    }

    DistanceNetProcessor::DistanceNetProcessor(bool convert_to_mesh_in) {
        init();
        convert_to_mesh=convert_to_mesh_in;
    }

    void get_normal(float p1x, float p1y,float p2x, float p2y, float res[]) {
        if (p1x == p2x) {
            res[0] = 0;
        } else {
            float slope = (p1y - p2y) / (p1x - p2x);
            res[0] = -1.f / slope;
        }
        if (p1y < p2y) {
            res[1] = -1;
        } else {
            res[1] = 1;
        }
    }



    void DistanceNetProcessor::execute() {
        auto tensor_coords = getInputData<Tensor>(0);
        auto tensor_distances = getInputData<Tensor>(1);

        auto coords_acces=tensor_coords->getAccess(ACCESS_READ_WRITE);
        auto raw_data_coords = coords_acces->getRawData();
        auto nb_coords = tensor_coords->getShape()[1];

        auto distances_acces=tensor_distances->getAccess(ACCESS_READ_WRITE);
        auto raw_data_distances = distances_acces->getRawData();
        auto nb_distances = tensor_distances->getShape()[0];

        std::vector<MeshVertex> vertices;
        std::vector<MeshLine> lines;
        std::vector<MeshTriangle> triangles;

        int total_nb_points = nb_coords+nb_distances;
        //float results[2*total_nb_points];
        float* results = new float[2*total_nb_points];

        auto OutputMesh = Mesh::create(vertices, lines, triangles);
        auto OutputMeshAccess = OutputMesh->getMeshAccess(ACCESS_READ_WRITE);
        for (int i = 0; i < nb_coords; i++) {
                auto x = raw_data_coords[i];
                auto y = raw_data_coords[i + nb_coords];
                OutputMeshAccess->addVertex(Vector3f(scale_factor_mesh *x, scale_factor_mesh *y, 0));
                results[i]=x;
                results[i+total_nb_points]=y;
        }
        // conversion of distances to coordinates
        auto x0 = raw_data_coords[0];
        auto y0 = raw_data_coords[nb_coords];
        auto x1 = raw_data_coords[nb_distances-1];
        auto y1 = raw_data_coords[nb_distances-1+nb_coords];
        auto angle_base = atan((y1-y0)/(x1-x0));
        auto norm_angle = angle_base+ + M_PI;
        auto distance_base_point_left=raw_data_distances[0];
        auto x = x0 + distance_base_point_left* cos(norm_angle);
        auto y = y0 + distance_base_point_left* sin(norm_angle);
        OutputMeshAccess->addVertex(Vector3f(scale_factor_mesh*x, scale_factor_mesh*y, 0));
        results[nb_coords]=x;
        results[nb_coords+total_nb_points]=y;
        // middle points
        auto prev2_lv_point_idx=0;
        auto prev_lv_point_idx=1;
        for (int i = 1; i < nb_distances-1; i++) {
            auto p1x = raw_data_coords[prev_lv_point_idx];
            auto p1y = raw_data_coords[prev_lv_point_idx+nb_coords];
            auto p2x = raw_data_coords[prev2_lv_point_idx];
            auto p2y = raw_data_coords[prev2_lv_point_idx+nb_coords];
            float r_dir[2];
            get_normal(p1x, p1y, p2x, p2y, r_dir);
            auto r = r_dir[0];
            auto dir = r_dir[1];
            auto angle= atan(r);

            if (dir==-1)
            {
                angle+=M_PI;
            }
            auto distance = raw_data_distances[i];
            auto x = p1x + distance* cos(angle);
            auto y = p1y + distance* sin(angle);
            OutputMeshAccess->addVertex(Vector3f(scale_factor_mesh *x, scale_factor_mesh *y, 0));
            prev2_lv_point_idx=prev_lv_point_idx;
            prev_lv_point_idx+=1;
            results[nb_coords+i]=x;
            results[nb_coords+i+total_nb_points]=y;
        }
        // right base point
        auto distance_base_point_right=raw_data_distances[nb_distances-1];
        x = x1 + distance_base_point_right* cos(angle_base);
        y = y1 + distance_base_point_right* sin(angle_base);
        OutputMeshAccess->addVertex(Vector3f(scale_factor_mesh *x, scale_factor_mesh *y, 0));
        results[total_nb_points-1]=x;
        results[2*total_nb_points-1]=y;
        std::initializer_list<int> shape_list = {2, total_nb_points};
        TensorShape tensor_shape(shape_list);
        auto tensor_results = Tensor::create(results,tensor_shape);
        tensor_results->setSpacing(tensor_coords->getSpacing());
        if (convert_to_mesh){
            addOutputData(0, OutputMesh);
        }
        else{
            addOutputData(0, tensor_results);
        }
    }
}