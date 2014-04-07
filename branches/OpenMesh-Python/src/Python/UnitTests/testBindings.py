import unittest
import openmesh

class TestIterators(unittest.TestCase):

    def setUp(self):
        self.mesh = openmesh.TriMesh()
        vh0 = self.mesh.add_vertex( openmesh.Vec3d(0, 0, 0) )
        vh1 = self.mesh.add_vertex( openmesh.Vec3d(1, 0, 0) )
        vh2 = self.mesh.add_vertex( openmesh.Vec3d(0, 1, 0) )
        self.mesh.add_face(vh0, vh1, vh2)

    def test_vertex_iterator(self):
        for v, i in zip(self.mesh.vertices, range(3)):
            self.assertEqual(v.idx(), i)

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(TestIterators)
    unittest.TextTestRunner(verbosity=2).run(suite)
