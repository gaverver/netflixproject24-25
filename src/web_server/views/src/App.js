import './App.css';
import { Routes, Route } from 'react-router-dom';
import DisconnectedHome from './pages/disconnected_home/Home';
import Admin from './pages/administration/Admin';
import MovieAdmin from './pages/administration/MovieAdmin';
import CategoryAdmin from './pages/administration/CategoryAdmin';
import SearchResults from './pages/searchResults/searchResults';
import Register from './pages/registration/Register';
import NotFound from './pages/page404/notFound';
import CategoriesPage from './pages/categories/categoriesPage';
import Login from './pages/login/Login'
import Watch from './pages/watchMovie/Watch';
import MovieInformation from './pages/movie_information/MovieInformation'

function App() {
  // router for all the pages in the application
  return (
    <div className="App">
      <Routes> 
        <Route path ="/home" element={<DisconnectedHome />} />
        <Route path ="/admin" element={<Admin />} />
        <Route path="/admin/movies" element={<MovieAdmin/>}/>
        <Route path="/admin/categories" element={<CategoryAdmin/>}/>
        <Route path="/movies/search/:query" element={<SearchResults />} />
        <Route path ="/register" element={<Register />} />
        <Route path ="/categories" element={<CategoriesPage />} />
        <Route path ="/login" element={<Login />} />
        <Route path="/movies/watch/:id" element={<Watch />} />
        <Route path="/movies/information/:id" element={<MovieInformation />} />
        <Route path ="/*" element={<NotFound />} />
      </Routes>
    </div>
  );
}


export default App;
