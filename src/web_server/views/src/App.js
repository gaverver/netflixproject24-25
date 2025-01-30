import './App.css';
import { Routes, Route } from 'react-router-dom';
import DisconnectedHome from './pages/disconnected_home/Home';
import SearchResults from './pages/searchResults/searchResults';
import Register from './pages/registration/Register';
import NotFound from './pages/page404/notFound';
import CategoriesPage from './pages/categories/categoriesPage';

function App() {
  // router for all the pages in the application
  return (
    <div className="App">
      <Routes> 
        <Route path ="/home" element={<DisconnectedHome />} />
        <Route path="/movies/search/:query" element={<SearchResults />} />
        <Route path ="/register" element={<Register />} />
        <Route path ="/categories" element={<CategoriesPage />} />
        <Route path ="/*" element={<NotFound />} />
      </Routes>
    </div>
  );
}


export default App;
