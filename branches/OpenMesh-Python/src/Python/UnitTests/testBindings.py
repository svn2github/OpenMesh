import unittest
import openmesh

class TestIterators(unittest.TestCase):

        def setUp(self):
                self.mesh = openmesh.TriMesh()
                self.vh0 = self.mesh.add_vertex( openmesh.Vec3d(0, 0, 0) )
                self.vh1 = self.mesh.add_vertex( openmesh.Vec3d(1, 0, 0) )
                self.vh2 = self.mesh.add_vertex( openmesh.Vec3d(0, 1, 0) )
                self.fh0 = self.mesh.add_face(self.vh0, self.vh1, self.vh2)

        def test_vertex_iterator(self):
                for v, i in zip(self.mesh.vertices, range(3)):
                        self.assertEqual(v.idx(), i)

        def test_vertex_vertex_iterator(self):
                for v, i in zip(self.mesh.vv(self.vh0), range(2, 1)):
                        self.assertEqual(v.idx(), i)

        def test_vertex_face_iterator(self):
                for f in self.mesh.vf(self.vh0):
                        self.assertEqual(f.idx(), self.fh0.idx())

        def test_face_vertex_iterator(self):
                for v, i in zip(self.mesh.fv(self.fh0), range(0, 2)):
                        self.assertEqual(v.idx(), i)


if __name__ == '__main__':
        suite = unittest.TestLoader().loadTestsFromTestCase(TestIterators)
        unittest.TextTestRunner(verbosity=2).run(suite)
